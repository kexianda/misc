package myudfs;

import java.io.IOException;
import java.util.ArrayList;
import java.util.Collections;
import java.util.Iterator;
import java.util.List;

import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.fs.FSDataInputStream;
import org.apache.hadoop.fs.FileSystem;
import org.apache.hadoop.fs.Path;
import org.apache.hadoop.mapreduce.Job;
import org.apache.pig.CollectableLoadFunc;
import org.apache.pig.ExecType;
import org.apache.pig.IndexableLoadFunc;
import org.apache.pig.builtin.PigStorage;
import org.apache.pig.data.BagFactory;
import org.apache.pig.data.DataBag;
import org.apache.pig.data.DefaultTuple;
import org.apache.pig.data.Tuple;
import org.apache.pig.impl.PigContext;


/**
 *      * A dummy loader which implements {@link IndexableLoadFunc} 
 *           */
    public class DummyIndexableLoader extends PigStorage implements IndexableLoadFunc {

        private String loc;
        private FSDataInputStream is;

        public DummyIndexableLoader() {
        }

        @Override
        public void close() throws IOException {
            is.close();
        }

        @Override
        public void seekNear(Tuple keys) throws IOException {

        }

        @Override
        public void initialize(Configuration conf) throws IOException {
            is = FileSystem.get(new Path(loc).toUri(), conf).open(new Path(loc));
        }

        @Override
        public void setLocation(String location, Job job) throws IOException {
            super.setLocation(location, job);
            loc = location;
        }

        @Override
        public Tuple getNext() throws IOException {
            String line = is.readLine();
            if(line == null)
                return null;
            String[] members = line.split("\t");
            DefaultTuple tuple = new DefaultTuple();
            for(String member : members)
                tuple.append(member);
            return tuple;
        }
    }
