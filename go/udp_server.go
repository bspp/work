package main
import (
	"fmt"
	"github.com/kimor79/gollectd"
	"net"
	"time"
	"errors"
)
var ErrorPacketsToLong = errors.New("packets to long")
var m = 0
func main() {
	// 创建监听
	socket, err := net.ListenUDP("udp4", &net.UDPAddr{
		IP:   net.IPv4(0, 0, 0, 0),
		Port: 8000,
	})
	if err != nil {
		fmt.Println("监听失败!", err)
		return

	}
	defer socket.Close()

	// 发送数据
	raddr, _ := net.ResolveUDPAddr("udp", "172.16.56.105:25826")
	for{
		Write(socket,raddr)
		time.Sleep(20*time.Second)
	}
}
type Packets struct {
	Packets []gollectd.Packet
	Data    []byte
}
func Write(socket *net.UDPConn,addr *net.UDPAddr){
	ps := NewPackets()
	for i := 0;i<2;i++{
		test := fmt.Sprintf("%s%d","test",i)
		packet := gollectd.Packet{}
		if(i % 10 == 0){
			packet = createMetric(test,"",200.00,100.00)
		}else{
			packet = createMetric(test,"usage",300.00)
		}
		_,n := ps.AddPackets(packet)
		if(n > 1452){
			socket.WriteToUDP(ps.Data,addr)
			ps.ClearPackets()
			ps.AddPackets(packet)
			time.Sleep(20*time.Millisecond)
			m++;
		}
	}
	
	socket.WriteToUDP(ps.Data,addr)
	ps.ClearPackets()
	m++
	fmt.Println(m)
}
func NewPackets() *Packets {
	return &Packets{
		Packets: make([]gollectd.Packet, 0),
	}
}
func (packets *Packets) ClearPackets() {
	packets.Packets = make([]gollectd.Packet, 0)
	packets.Data = make([]byte, 0)

}
func (packets *Packets) AddPackets(packet gollectd.Packet) (error, int) {
	ps := append(packets.Packets, packet)
	data, err := gollectd.UnPackets(&ps)
	if err != nil {
		return err, 0
	}
	if len(data) > 1452 {
		return ErrorPacketsToLong, len(data)
	}
	packets.Packets = ps
	packets.Data = data
	return  nil,1
}
func msToCollecdTime(ms uint64) uint64 {
	return ((((uint64)(ms)) / 1000) << 30) | ((((((uint64)(ms)) % 1000) << 30) + 500) / 1000)
}
func createMetric(ty string,type_instance string,values ...float64)gollectd.Packet{
	//timeHR from metic
	timeHR := msToCollecdTime(uint64(time.Now().UnixNano()/ 1e6))
	//set Values
	vls := gollectd.NewValues()
	for _,value := range values{
		vls.AddValue("value","gauge",value)
	}
	intervalHR := msToCollecdTime(uint64(10 * 1000))
	packet := gollectd.NewPacket("cluster_datacenter1_hostname", intervalHR, "", "", timeHR, ty, vls.Values)
	packet.TypeInstance = type_instance
	return *packet
}

