#### Building the kernel

```
	$ git clone --depth 100 --branch Ubuntu-5.4.0-42.46 git://kernel.ubuntu.com/ubuntu/ubuntu-focal.git
	$ sudo apt install bison flex libelf-dev fakeroot build-essential crash kexec-tools makedumpfile kernel-wedge libncurses5 libncurses5-dev libelf-dev asciidoc binutils-dev libudev-dev curl zfs-dkms libcap-dev
	$ cd ubuntu-focal
	// $ LANG=C fakeroot debian/rules distclean
	$ LANG=C fakeroot debian/rules clean
	// $ LANG=C fakeroot debian/rules editconfigs
	$ cp /boot/config-5.4.0-42-generic build/.config
	$ sed -Ei 's/(CONFIG_VERSION_SIGNATURE).*/\1="Ubuntu 5.4.0-43.0+mba"/g' build/.config
	$ sed -Ei 's/(CONFIG_MODULE_SIG_ALL)=[ynm]/\1=n/g' build/.config
```
* Disable automatic modules signing and save as `.config`
	* This needs also a change in policy: `debian.master/config/annotations` (`CONFIG_MODULE_SIG_ALL: y->n`)

* Make the kernel newer than the stock one in `debian.master/changelog`:`linux
  (5.4.0-[43.+mba20200723]) focal; urgency=medium` increment the [] part

* Disable some components as their paths are broken (fixable with pkgs.org?):
	$ sed -Ei 's/(install-%: enable_zfs = ).*/\1false/' debian/rules.d/2-binary-arch.mk
	$ for f in debian.master/rules.d/*.mk debian/rules; do sed -Ei 's/(do_dkms_nvidia)[ ]*=[ ]*true/\1=false/g' $f; done
	$ for f in debian.master/rules.d/*.mk debian/rules; do sed -Ei 's/(do_dkms_nvidia_server)[ ]*=[ ]*true/\1=false/g' $f; done
	$ for f in debian.master/rules.d/*.mk debian/rules; do sed -Ei 's/(do_dkms_wireguard)[ ]*=[ ]*true/\1=false/g' $f; done
	$ for f in debian.master/rules.d/*.mk debian/rules; do sed -Ei 's/(do_extras_package)[ ]*=[ ]*true/\1=false/g' $f; done
	$ for f in debian.master/rules.d/*.mk debian/rules; do sed -Ei 's/(do_tools_perf_jvmti)[ ]*=[ ]*true/\1=false/g' $f; done

* In `debian.master/abi` rename the directory of older version accordingly to
  previous version from `changelog`, eg:
	$ cd debian.master/abi && mv 5.4.0-41.45 5.4.0-42.46
* Ignore some modules:
cat <<@ >> debian.master/abi/5.4.0-42.46/modules.ignore
icp
spl
vboxguest
vboxsf
wireguard
zavl
zcommon
zfs
zlua
znvpair
zunicode
@


* Launch the build
```
	$ LANG=C fakeroot debian/rules binary-headers binary-generic binary-perarch
```

* If needed to re-launch remove these directories:
	$ rm -rf debian/linux-modules-extra-5.4.0-43-generic/lib/modules/5.4.0-43-generic/kernel/kernel/*

* After the build:
	$ sudo cp debian/linux-modules-5.4.0-43-generic/boot/System.map-5.4.0-43-generic /usr/src/linux-headers-5.4.0-43-generic/System.map

* Install:
	$ cd ..
	$ sudo dpkg -i linux-*-5.4.0-43*.deb
	$ sudo dpkg -i linux-headers-5.4.0-43_5.4.0-43.0_all.deb
	$ sudo dpkg -i linux-headers-5.4.0-43-generic_5.4.0-43.0_amd64.deb
	$ sudo dpkg -i linux-modules-5.4.0-43-generic_5.4.0-43.0_amd64.deb

REBOOT

* Edit: /usr/src/linux-headers-5.4.0-43/scripts/depmod.sh


#### Building and loading the module
	$ ln -s /boot/System.map-5.4.0-43-generic System.map
	$ make modules
	$ make modules_install

