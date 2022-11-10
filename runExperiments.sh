SEED=42

SIZE=100
RANGE=100

for i in {1..4}; do
  SIZE=100
  for j in {1..4}; do
    ./run.sh $SEED $SIZE -$RANGE $RANGE
    SIZE=$((SIZE * 100))
  done
  RANGE=$((RANGE * 100))
done
