# Unzip all cylindrical droplets

node=n025

./parallel.sh $SCRATCH/droplet/exec/unzipfile.sh unzipCyl20A $node $SCRATCH/droplet/data/Bob/Sub951By50/Cyl20A/atom{} '' '' 1 1
./parallel.sh $SCRATCH/droplet/exec/unzipfile.sh unzipCyl30A $node $SCRATCH/droplet/data/Bob/Sub951By50/Cyl30A/atom{} '' '' 1 5
./parallel.sh $SCRATCH/droplet/exec/unzipfile.sh unzipCyl40A $node $SCRATCH/droplet/data/Bob/Sub951By50/Cyl40A/atom{} '' '' 1 5
#./parallel.sh $SCRATCH/droplet/exec/unzipfile.sh unzipCyl60A_atom{} $node $SCRATCH/droplet/data/Bob/Sub951By50/Cyl60A/atom{} '' '' 1 5
./parallel.sh $SCRATCH/droplet/exec/unzipfile.sh unzipCyl100A $node $SCRATCH/droplet/data/Bob/Sub951By50/Cyl100A/atom{} '' '' 1 5

