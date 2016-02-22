package myudfs;

import java.io.IOException;
import org.apache.pig.CollectableLoadFunc;
import org.apache.pig.builtin.PigStorage;
import org.apache.pig.data.BagFactory;
import org.apache.pig.data.DataBag;
import org.apache.pig.data.Tuple;

public class DummyCollectableLoader extends PigStorage implements CollectableLoadFunc{

	@Override
		public void ensureAllKeyInstancesInSameSplit() throws IOException {
		}

}
