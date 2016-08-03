////////////////////////////////////////////////////////////////////////
//    Root macro to visualize Geant4 simulation data (SWG2013)        //
//    Oct-2013 E. Nacher                                              //
////////////////////////////////////////////////////////////////////////

{
	//Variables to read (and others...)

	char InputFileName[60], ;
	int NumberOfChannels = 10000;
	int Histo[NumberOfChannels];
	float dummy;
	
	system("clear");
	system("ls -tal *.vec");
	cout << "Enter input file name: ";
	cin >> InputFileName;
	ifstream InputFile(InputFileName);

	if(InputFile.good() == 0){
	cout << "No such file!!" << endl;
	return 0;
	}

	//Initialize arrays to zero

	for(i=0; i < NumberOfChannels ; i++){
	Histo[i]=0;
	}

	//Reading and processing data 
	
	i=0;
	cout << endl << endl;

	cout << "Processing data..." << endl;

	TH1F* RootHist = new TH1F(" ","",10000,0,1000.);

	while(i < NumberOfChannels && InputFile.good()){
	    InputFile >> i >> Histo[i];
	    RootHist->SetBinContent(i,Histo[i]);
	}		
	
	
// set default graphics options
	
	gROOT->SetStyle("Plain");
	gStyle->SetOptStat(1000011);
	gStyle->SetPalette(1);
	
	RootHist->GetXaxis()->SetTitleSize(0.035);
	RootHist->GetXaxis()->SetLabelSize(0.035);
	RootHist->GetXaxis()->SetTitle("Time (ns)");

	RootHist->GetYaxis()->SetTitleSize(0.035);
	RootHist->GetYaxis()->SetLabelSize(0.035);
	RootHist->GetYaxis()->SetTitleOffset(1.4);
	RootHist->GetYaxis()->SetTitle("Counts");
	
	RootHist->SetLineWidth(2);
	RootHist->SetLineColor(2);
	RootHist->Draw();

}