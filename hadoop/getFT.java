import java.io.IOException;
import java.util.*;

import org.apache.hadoop.fs.Path;
import org.apache.hadoop.io.*;
import org.apache.hadoop.mapred.*;
import org.apache.hadoop.mapred.Mapper;
import org.apache.hadoop.mapred.Reducer;


public class getFT {

	public static class Map extends MapReduceBase implements Mapper<LongWritable, Text, Text, IntWritable> {
		private final static IntWritable one = new IntWritable(1);
		private Text word = new Text();

		public void map(LongWritable key, Text value, OutputCollector<Text, IntWritable> output, Reporter reporter) throws IOException {
			String line = value.toString();
			String[] columns = line.split(",");
			
			int year = 0;
			//check if the row is a header
			try{
				year = Integer.parseInt(columns[0]);
			}
			catch(Exception e){
				return;
			}
			//files before 2008, read column 15
			if(year < 2008){
				if(columns[14] == "NA") return;
				word.set(columns[14]);	
				output.collect(word, one);
				return;
			}
			//files after 2008, read column 45
			else{
				double delay = 0;
				try{
					delay = Double.parseDouble(columns[44]);
				}
				catch(Exception e){
					return;
				}
				word.set(Integer.toString((int)delay));
				output.collect(word, one);
				return;
			}
			
		}
	}

	public static class Reduce extends MapReduceBase implements Reducer<Text, IntWritable, Text, IntWritable> {
		public void reduce(Text key, Iterator<IntWritable> values, OutputCollector<Text, IntWritable> output, Reporter reporter) throws IOException {
			int sum = 0;
			while (values.hasNext()) {
				sum += values.next().get();
			}
			output.collect(key, new IntWritable(sum));
		}
	}

	public static void main(String[] args) throws Exception {
		JobConf conf = new JobConf(getFT.class);
		conf.setJobName("getFrequencyTable");
		//conf.setNumMapTasks(8);

		conf.setOutputKeyClass(Text.class);
		conf.setOutputValueClass(IntWritable.class);

		conf.setMapperClass(Map.class);
		conf.setReducerClass(Reduce.class);

		conf.setInputFormat(TextInputFormat.class);
		conf.setOutputFormat(TextOutputFormat.class);

		FileInputFormat.setInputPaths(conf, new Path(args[0]));
		FileOutputFormat.setOutputPath(conf, new Path(args[1]));

		JobClient.runJob(conf);
	}
}
