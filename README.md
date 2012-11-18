Filter graph
------------

> Connecting processing blocks

Sample code:

	Filter* source;
	Filter* sink;
	// connect source's output to sink's input
	source->GetPort("output")->Connect(sink->GetPort("input"));
	// main loop
	while (true) {
	  // data output from source will be sent to sink for processing automatically
	  source->Process();
	}
