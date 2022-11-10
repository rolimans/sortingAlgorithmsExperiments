gcc -o main main.c -pg -lm

SEED=$1
SIZE=$2
MIN=$3
MAX=$4

experimentName="experiment_${SEED}_${SIZE}_${MIN}_${MAX}"
details="Seed: ${SEED}, Size: ${SIZE}, Min: ${MIN}, Max: ${MAX}"

echo "Running experiment $experimentName"

mkdir -p experiments/$experimentName

stdbuf --output=L ./main $SEED $SIZE $MIN $MAX | tee experiments/$experimentName/output.txt

gprof -PcleanCache -PgetRandomNumber -PisSorted -ParraysHaveTheSameElements -PgetRandomArray -QcleanCache -QgetRandomNumber -QisSorted -QarraysHaveTheSameElements -QgetRandomArray -NcleanCache -NgetRandomNumber -NisSorted -NarraysHaveTheSameElements -NgetRandomArray -b main gmon.out >experiments/$experimentName/profile.txt

gprof2dot --node-label=self-time --node-label=total-time --node-label=self-time-percentage --node-label=total-time-percentage -n 0 -e 0 experiments/$experimentName/profile.txt >tmp.dot

sed -i "2i label=\"${details}\";" tmp.dot
sed -i "3i labelloc=t;" tmp.dot

dot tmp.dot -Tpng -o experiments/$experimentName/profile.png

rm gmon.out
rm tmp.dot

echo "Experiment $experimentName finished"
