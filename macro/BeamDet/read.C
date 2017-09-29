void read(){
  TString fParamsXmlFileName = "equip.xml";
  TDOMParser *domParser = new TDOMParser();

  Int_t parsecode = domParser->ParseFile(fParamsXmlFileName);

  if (parsecode < 0) {
     cerr << domParser->GetParseCodeMessage(parsecode) << endl;
  }

  TXMLNode *rootNode = domParser->GetXMLDocument()->GetRootNode();
  TXMLNode *detPartNode = rootNode->GetChildren();

  for ( ; detPartNode; detPartNode = detPartNode->GetNextNode()) {
	cout << detPartNode->GetNodeName() << endl;
	TXMLNode *curNode = detPartNode->GetChildren();
	for ( ; curNode; curNode = curNode->GetNextNode()){
		cout << "\t" << curNode->GetNodeName() << endl;
		TXMLNode *curNode2 = curNode->GetChildren();
		for ( ; curNode2; curNode2 = curNode2->GetNextNode()){
			cout << "\t\t" << curNode2->GetNodeName() << endl;
			if (curNode2->HasAttributes()){
				TList *attrList = curNode2->GetAttributes();
				TIter next(attrList);
				TXMLAttr* attr;
		        while (attr=(TXMLAttr*)next()) {
		        	cout << "\t\t\t" << attr->Key() << " = " << attr->GetValue() << endl;
		        }
			}
			else if(curNode2->GetText() ){
				cout << "\t\t\t" << curNode2->GetText() << endl;
			}
		}
	}
  }
}