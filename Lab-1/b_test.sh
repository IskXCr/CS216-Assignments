i=0
while true;
do
    ./b_gen > ./b.sample.in
    ./b_test < ./b.sample.in > test.out
    if [ $? -eq 1 ]; then
        break
    fi
    sleep 1
    echo -ne "\rloop: $i"
    i=`expr $i + 1`
done
echo -e "\nfound."
cat test.out