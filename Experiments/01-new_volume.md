hdiutil uses the DiskImages framework to manipulate disk images. 

Disk images are data containers that emulate disks.  Like disks, they can be partitioned and formatted.

## Creating a disk image 
```
hdiutil create -fs APFS -size 200MB 200mb.apfs
created: /private/tmp/200mb.apfs.dmg
```

## Attaching the image 

The terms "attach" and "detach" are used to distinguish the way disk images are connected to and disconnected from a system.  
"Mount" and "unmount" are the parallel filesystems options.  

* As you can see, when attaching the image it has created two disks: _disk3_ and _disk4_. 
* _**disk3**_ represents the image (dmg) that we created. 
* _**disk4**_ is a "synthesized" disk aka the APFS container. 
* _**/dev/disk4s1**_ is a volume that has been automatically created under the APFS container (_disk4_). This volume is mounted under _**/Volumes/untitled**_

```
hdiutil attach 200mb.apfs.dmg 
/dev/disk3          	GUID_partition_scheme          	
/dev/disk3s1        	Apple_APFS                     	
/dev/disk4          	EF57347C-0000-11AA-AA11-0030654	
/dev/disk4s1        	41504653-0000-11AA-AA11-0030654	/Volumes/untitled
```

## Where is the APFS container?

* We can run the `mmls` command from The Sleuthkit toolset to get the location of the APFS container inside the dmg file. 
* In this example, look at row "004" wit the description **disk image**. This is located at an offset of 40 from the beginning of the image. 
* The offset is given in units of 512-byte sectors (also shown in the output). So the actual offset is 40*512=20480. 

```
mmls ./200mb.apfs.dmg
GUID Partition Table (EFI)
Offset Sector: 0
Units are in 512-byte sectors

      Slot      Start        End          Length       Description
000:  Meta      0000000000   0000000000   0000000001   Safety Table
001:  -------   0000000000   0000000039   0000000040   Unallocated
002:  Meta      0000000001   0000000001   0000000001   GPT Header
003:  Meta      0000000002   0000000033   0000000032   Partition Table
004:  000       0000000040   0000409559   0000409520   disk image
005:  -------   0000409560   0000409599   0000000040   Unallocated
```

## Additional notes
* After running , a disk image (dmg) becomes a disk. You can use the diskutil commands with disk3/disk4. Example:  
```
diskutil list disk3
/dev/disk3 (disk image):
   #:                       TYPE NAME                    SIZE       IDENTIFIER
   0:      GUID_partition_scheme                        +209.7 MB   disk3
   1:                 Apple_APFS Container disk4         209.7 MB   disk3s1



diskutil list disk4
/dev/disk4 (synthesized):
   #:                       TYPE NAME                    SIZE       IDENTIFIER
   0:      APFS Container Scheme -                      +209.7 MB   disk4
                                 Physical Store disk3s1
   1:                APFS Volume untitled                24.6 KB    disk4s1
```
* If multiple disk images get mounted as disks, how will you know which of these is the physical disk and which of these is virtual? You can use `diskutil list physical` - for listing physical disks and `diskutil list virtual` - for listing virtual disks. 

* If you want APFS specific information for a disk you can use `diskutil apfs list <diskname>`. Note: This will work only for synthesized disks that are APFS containers. 
```
diskutil apfs list disk3
disk3 is not an APFS Container


diskutil apfs list disk4
|
+-- Container disk4 4820F4FD-366A-43E0-AA67-C123C7AD3FA6
    ====================================================
    APFS Container Reference:     disk4
    Size (Capacity Ceiling):      209674240 B (209.7 MB)
    Capacity In Use By Volumes:   1486848 B (1.5 MB) (0.7% used)
    Capacity Not Allocated:       208187392 B (208.2 MB) (99.3% free)
    |
    +-< Physical Store disk3s1 E9F72AC6-59D3-43B1-AAC0-216E744CCD30
    |   -----------------------------------------------------------
    |   APFS Physical Store Disk:   disk3s1
    |   Size:                       209674240 B (209.7 MB)
    |
    +-> Volume disk4s1 F2CD8F43-1651-4AC3-B0A3-9D681EF68123
        ---------------------------------------------------
        APFS Volume Disk (Role):   disk4s1 (No specific role)
        Name:                      untitled (Case-insensitive)
        Mount Point:               /Volumes/untitled
        Capacity Consumed:         24576 B (24.6 KB)
        FileVault:                 No
```



```
file 200mb.apfs.dmg 
200mb.apfs.dmg: DOS/MBR boot sector; partition 1 : ID=0xee, start-CHS (0x3ff,254,63), end-CHS (0x3ff,254,63), startsector 1, 409599 sectors, extended partition table (last)
```
