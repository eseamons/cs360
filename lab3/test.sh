

 #!/bin/bash
for i in $(seq 0 40)
do 
    GET -dUe http://ec2-52-26-232-1.us-west-2.compute.amazonaws.com:3001/ &
done