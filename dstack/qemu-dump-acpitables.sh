 #!/bin/sh
 for N in `seq 46 128`; do
 echo "N=$N"

../build/qemu-system-x86_64 -accel kvm -cpu host -smp $N -m 2048M -nographic -nodefaults -serial stdio -kernel /home/kvin/codes/meta-dstack/live-0/images/dstack-dev-0.3.3/bzImage -initrd /home/kvin/codes/meta-dstack/live-0/images/dstack-dev-0.3.3/initramfs.cpio.gz -drive file=./hda.img,if=none,id=hd0 -device virtio-blk-pci,drive=hd0 -cdrom /home/kvin/codes/meta-dstack/live-0/images/dstack-dev-0.3.3/rootfs.iso -bios /home/kvin/codes/meta-dstack/live-0/images/dstack-dev-0.3.3/ovmf.fd -netdev user,id=net0,net=10.0.2.0/24,dhcpstart=10.0.2.10,restrict=no -device virtio-net-pci,netdev=net0 -machine q35,kernel-irqchip=split,confidential-guest-support=tdx,hpet=off -object tdx-guest,id=tdx -device vhost-vsock-pci,guest-cid=120102 -virtfs local,path=/home/kvin/codes/meta-dstack/live-0/run/vm/57664a88-6ef3-4d58-9881-a707f673af5e/shared,mount_tag=host-shared,readonly=on,security_model=mapped,id=virtfs0 -append "console=ttyS0 init=/init dstack.fde=1 panic=1 systemd.unified_cgroup_hierarchy=0" > $N.tbl

done
