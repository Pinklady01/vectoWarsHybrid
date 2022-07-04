REM Test the vectorwar sample by starting 2 clients connected
REM back to each other.
REM
REM Controls: Arrows to move
REM           Press 'D' to fire
REM           Press 'P' to show performance monitor
REM           Shift to strafe

pushd
cd cmake-build-debug
start VectorWarHybrid.exe 7000 2 127.0.0.1:7001 local
popd