---
title: Maggot - Enable AVX
summary: 
authors:
    - Daniel Jacob
date: 2023-10-09
some_url:
---

# Enable AVX

<style>.md-typeset h1 {display: none;} .md-nav__item {font-size: medium}</style>

<style>.md-typeset table:not([class]) th {
    background-color: var(--md-default-fg-color--light);
    color: var(--md-default-bg-color); min-width: 5rem; padding: 0.9375em 1.25em; vertical-align: top;}</style>

### Enable AVX in VirtualBox under Windows 11

From MongoDB version 5.0 and higher, your guest machine must support [AVX][1]{:target="_blank"}. To enable AVX on virtual machines in VirtualBox under Windows 11, you must disable [Hyper-V][2]{:target="_blank"}. Which is not obvious because usually trying to disable Hyper-V by unchecking the "Windows Features" boxes often does not completely disable Hyper-V.

To turn Hyper-V off completely, see this post : [HMR3Init: Attempting fall back to NEM (Hyper-V is active)][4]{:target="_blank"}


To know if your Linux virtual machine supports AVX, run the command ```cat /proc/cpuinfo``` , then check that in the 'flags', you have 'avx' and 'avx2'.


#### Summary of the steps to follow (You must have administrator rights)

* cmd window: 

    ```bcdedit /set hypervisorlaunchtype off```

* Regedit:

    ```Computer\HKEY_local_machine\system\CurrentControlSet\Control\DeviceGuard\EnableVirtualizationBaseSecurity <- 0```

    ```Computer\HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Control\DeviceGuard\Scenarios\SystemGuard <- 0```

* Rebooted the host machine (i.e. Windows)


* Then, you need to enable AVX for the corresponding virtual machine ("Maggot VM")

    ```
    VBoxManage.exe setextradata <Maggot VM> VBoxInternal/CPUM/IsaExts/AVX 1 
    VBoxManage.exe setextradata <Maggot VM> VBoxInternal/CPUM/IsaExts/AVX2 1 
    ```


[1]: https://en.wikipedia.org/wiki/Advanced_Vector_Extensions
[2]: https://learn.microsoft.com/en-us/virtualization/hyper-v-on-windows/about/
[3]: https://forums.virtualbox.org/viewtopic.php?t=109776
[4]: https://forums.virtualbox.org/viewtopic.php?f=25&t=99390

