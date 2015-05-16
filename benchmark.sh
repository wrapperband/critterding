# > benchmark result : 
# >   frames      :5000
# >   time        :21.077 seconds
# >   seed        :13
# >   checksum    :1094.2192
# >   average fps :237.3203 fps


# > benchmark result : 
# >   frames      :5000
# >   time        :20.893 seconds
# >   seed        :13
# >   checksum    :1094.2192
# >   average fps :239.41033 fps

# ./src/critterding --glsl 1 --roundworld 1 --worldsizeX 50 --autoload 1 --benchmark 1 --benchmark_frames 5000 --startseed 13 --threads 3 --energy 400 | grep '>'



#############################################################################"
# > benchmark result : 
# >   frames      :3000
# >   time        :12.023 seconds
# >   seed        :13
# >   checksum    :4080.7397
# >   average fps :249.72964 fps

# 	./src/critterding --glsl 1 --roundworld 1 --worldsizeX 30 --benchmark 1 --benchmark_frames 3000 --startseed 13 --threads 2 --energy 1000 --mincritters 4 | grep '>'
#############################################################################"


#############################################################################"
# > benchmark result : 
# >   frames      :8000
# >   time        :59.933 seconds
# >   seed        :13
# >   checksum    :7472.5518
# >   average fps :133.51576 fps

# 	./src/critterding --glsl 1 --roundworld 1 --worldsizeX 40 --benchmark 1 --benchmark_frames 8000 --startseed 13 --threads 2 --energy 1000 --mincritters 40 | grep '>'
#############################################################################"



#############################################################################"
# > benchmark result : 
# >   frames      :3000
# >   time        :12.023 seconds
# >   seed        :13
# >   checksum    :4080.7397
# >   average fps :249.72964 fps

# 	./src/critterding --glsl 1 --roundworld 0 --benchmark 1 --benchmark_frames 2000 --startseed 130 --threads 4 --energy 10 --mincritters 80 --drawscene 0 | grep '>'
	./src/critterding --glsl 1 --roundworld 0 --benchmark 1 --benchmark_frames 2000 --startseed 130 --threads 4 --energy 10 --mincritters 80 --drawscene 0
#############################################################################"

