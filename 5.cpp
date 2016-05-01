/*EXP 5
macro
HARIPRASAD K
30/3/2016
*/

#include<iostream>
#include<sstream>
#include<fstream>
#include<map>
#include<list>

using namespace std;

ifstream inFile;
ofstream outFile;

class Line{public:
	string label,opCode,operand;
	Line(string l,string o,string op): label(l),opCode(o),operand(op) {}
	string toString(){
		return label+"\t"+opCode+"\t"+operand+'\n';
	}
};

map<string,list<Line> >defTab;

Line readLine(){
	string l,o,op;
	getline(inFile,l,'\t');
	getline(inFile,o,'\t');
	getline(inFile,op);
	return Line(l,o,op);
}


void define(Line line){
	int level=1;
	defTab[line.label].push_back(line);
	while(level>0){
		Line dLine = readLine();
		if(dLine.opCode=="MACRO")
			level=level+1;
		else if(dLine.opCode=="MEND")
			level=level-1;
		else
			defTab[line.label].push_back(dLine);
	}
}

void expand(Line line){
	outFile<<"."<<line.toString();

	list<Line> def = defTab[line.opCode];

	list<Line>::iterator it=def.begin();

	stringstream from(def.begin()->operand);
	stringstream to(line.operand);
	map<string,string> replacement;
	string f,t;
	while(getline(from,f,',')&&getline(to,t,',')){
		replacement[f]=t;
	}

	list<Line>::iterator i = def.begin();
	for(++i;i!=def.end();i++){
		string w = i->toString();
		for(map<string,string>::iterator j=replacement.begin();j!=replacement.end();j++){
			int pos = w.find(j->first);
			while(pos!=-1){
				w.replace(pos,j->first.size(),j->second);
				pos = w.find(j->first);

			}
		}
		outFile<<w;
	}
}


void processLine(Line line){
	if(line.opCode=="MACRO")
		define(line);
	else if(defTab.find(line.opCode)!=defTab.end())
		expand(line);
	else
		outFile<<line.toString();

}

int main(int argc,char* argv[]){
	if(argc<2){
		cout<<"invalid input"<<endl;
		return 0;
	}
	inFile.open(argv[1]);
	outFile.open((string(argv[1])+".out").c_str());
	Line line("","","");
	while((line=readLine()).opCode!="END"){
		processLine(line);
	}
	cout<<"Macro processed successfully"<<endl;
	return 0;
}
