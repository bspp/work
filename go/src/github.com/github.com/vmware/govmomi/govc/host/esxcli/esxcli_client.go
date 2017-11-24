/*
Copyright (c) 2014-2015 VMware, Inc. All Rights Reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

package esxcli

import (
	"flag"

	"golang.org/x/net/context"

	"github.com/vmware/govmomi/govc/flags"
)

type Esxcli struct {
	*flags.HostSystemFlag

	hints bool
}

func (cmd *Esxcli) Register(ctx context.Context, f *flag.FlagSet) {
	cmd.HostSystemFlag, ctx = flags.NewHostSystemFlag(ctx)
	cmd.HostSystemFlag.Register(ctx, f)

	f.BoolVar(&cmd.hints, "hints", true, "Use command info hints when formatting output")
}

func (cmd *Esxcli) Process(ctx context.Context) error {
	if err := cmd.HostSystemFlag.Process(ctx); err != nil {
		return err
	}
	return nil
}


func (cmd *Esxcli) Run(ctx context.Context, f *flag.FlagSet) (*Response, error) {
	c, err := cmd.Client()
	if err != nil {
		return nil, err
	}

	host, err := cmd.HostSystem()
	if err != nil {
		return nil, err
	}

	e, err := NewExecutor(c, host)
	if err != nil {
		return nil, err
	}

	res, err := e.Run(f.Args())
	if err != nil {
		return nil, err
	}

	if len(res.Values) == 0 {
		return res, nil
	}

	return res, nil
}
