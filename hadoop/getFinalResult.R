# Read the output file(frequency table) from Hadoop and calculate the mean, sd and median
# Timer starts here
start <- proc.time()
uniq <- read.csv("output.txt", header = FALSE, sep = "")
attach(uniq)
uniq <- uniq[order(V1),]
detach(uniq)
# Remove last row (NA)
row = nrow(uniq)
uniq <- uniq[-row,]
row <- row-1
# Data size
n = sum(uniq[,2])

# Calculate average
SumDelay = 0
for (i in 1:row){
  SumDelay =  SumDelay + uniq[i,1] * uniq[i,2]
}
Mean = SumDelay/n

# Calculate standard variation
Var = 0
for (i in 1:row){
  Var = Var + (uniq[i,1] - Mean)^2 * uniq[i,2]
}
SD = sqrt(Var/n)

# Find the median, the average of values of index mid1 and mid2
if(n%%2 == 0){
  mid1 = n/2
  mid2 = mid1 + 1
}
if(n%%2 == 1){
  mid1 = n/2 + 0.5
  mid2 = mid1
}
# Use t1, t2 to track current index
t1 = 0
t2 = uniq[1,2]
for (i in 1:(row-1)){
  if(t1 < mid1 && t2 >= mid1)
    m1 = uniq[i,1]
  if(t1 < mid2 && t2 >= mid2){
    m2 = uniq[i,1]
    break
  }
  t1 = t2
  t2 = t2 + uniq[i+1,2]
}
Median = (m1 + m2)/2
# Timer stops here
time = proc.time()-start
# Result: Mean = 6.5665 SD = 31.5563 Median = 0 time = 0.312s