#include<bits/stdc++.h>
using namespace std;

long long int f1(vector<string>&M, unsigned long long int n, int a=0){
	int siz = M.size();
	int count[siz][siz];
	unsigned long long int prev[siz], curr[siz];
	for (int i=0; i<siz; i++){
		for (int j=0; j<siz; j++){
			count[i][j]=0;
		}
	}	
	for (int i=0; i<siz; i++){
		for (int j=0; j<M[i].size(); j++){
			count[i][M[i][j]-'a']++;
		}
	}	
	unsigned long long int ans=0;
	//string x=M[a];
	for (int i=0; i<siz; i++)prev[i]=0;
	prev[a]=1;	
	//for(int i=0; i<x.size(); i++)prev[x[i]-'a']++;
	for (int i=0; i<n; i++){
		for (int j=0; j<siz; j++)curr[j]=0;
		for (int j=0; j<siz; j++)
			for (int k=0; k<siz; k++) curr[k]+=prev[j]*count[j][k];

		for (int j=0; j<siz; j++)
			prev[j]=curr[j];
	}
	for(int i=0; i<siz; i++)ans+=prev[i];
	return ans;

}

void help(vector<string>&M, vector< vector<bool> > b, char c, int n, string &w, int &l, int &length){
	if (l==w.size()) return ;	
	if (n==0){length++;
		if (w[l]==c) l++;
		return;
	}
	for (int i=0; i<M[c-'a'].size(); i++){
		if(b[M[c-'a'][i]-'a'][w[l]-'a']){
				help(M, b, M[c-'a'][i], n-1, w, l, length);
			}
			else{
				length+=f1(M,n,M[c-'a'][i]-'a');
	
			}
		if (l==w.size()) return;
	}
}

char f2(vector<string>&M, unsigned long long int n, int a=0, int i=-1){
	if (i==0) return char('a'+a);		
	char rec;	
	if (i==-1){
		if (n==0) return 'a';
		i=0;	
		while(n>=f1(M, i, a)) i++;
	}
	for (int j=0; j<M[a].size(); j++){
		unsigned long long int an = f1(M, i-1, M[a][j]-'a');		
		if (n<an) {rec = M[a][j]; break;}		
		n -= an;
	}
	return f2(M, n, rec-'a', i-1);
}

void inverse(vector<string>&M, string &w, vector<pair<string, vector<string> > > &ans, vector<string> pre, string rev="", int st=0){		
	if(st==w.size()) {ans.push_back(make_pair(rev, pre)); return;}	
	int siz=M.size();	
	if (st==0){
		for(int i=0; i<siz; i++){
			int j=M[i].size();			
			if(j>w.size()) {
				for (int k=0; k<=M[i].size()-w.size(); k++){
					if (w==M[i].substr(k,w.size())){
						vector<string>pre1=pre;
						pre1.push_back(M[i].substr(0,k));						
						inverse(M, w, ans, pre1, rev+char('a'+i), w.size());
						break;	
					}
				}
			}
			for (; j>0; j--){
				if(w.substr(0, j)==M[i].substr(M[i].size()-j, j)){
					vector<string>pre1=pre;
					pre1.push_back(M[i].substr(0,M[i].size()-j));
					inverse(M, w, ans, pre1, rev+char('a'+i), j); 						
				}
			}
		}
		return;
	}
	int k=w.size()-st;
	for(int i=0; i<siz; i++){		
		if(k<M[i].size()){
			if (w.substr(st, k)==M[i].substr(0,k)){ 				
				inverse(M, w, ans, pre, rev+char('a'+i), st+k);				
			}
		}
		else{
			int l=M[i].size();
			if(w.substr(st, l)==M[i]){
				inverse(M, w, ans, pre, rev+char('a'+i), st+l);				
			}	
		}	
	}

}


void f4(vector<string>&M, string &w){
	int l=0, siz=M.size();
	vector< vector<bool> > b;
	for (int i=0; i<siz; i++){
		vector<bool>ok;
		for (int j=0; j<siz; j++)
			ok.push_back(0);
		b.push_back(ok);
	}
	for (int i=0; i<siz; i++){
		set<char>found, new1, new2; int k=0;
		for (int j=0; j<M[i].size(); j++){
			new1.insert(M[i][j]);
		}
		while(!new1.empty()){
			for (set<char>::iterator it=new1.begin(); it!=new1.end(); it++){
				found.insert(*it);
			}
			for (set<char>::iterator it=new1.begin(); it!=new1.end(); it++){
				for (int k=0; k<M[*it-'a'].size(); k++){
					if (found.find(M[*it-'a'][k])==found.end())new2.insert(M[*it-'a'][k]);
				}
			}
			new1=new2;
			new2.clear();
		}
		for (set<char>::iterator it=found.begin(); it!=found.end(); it++)
			b[i][*it-'a']=1;
	} 
	bool bo=1;
	int n=0, length=1;
	if (w[0]=='a')l++;
	string x=M[0].substr(1);
	while(l<w.size() && bo){
		bo=0;		
		for (int i=0; i<x.size(); i++){
			if(b[x[i]-'a'][w[l]-'a']){
				bo=1;
				help(M, b, x[i], n, w, l, length);
			}
			else{
				length+=f1(M,n,x[i]-'a');
	
			}						
			if (l==w.size()) break;
		}
		n++;				
	}
	if (!bo) cout << -1 << "\n";
	else {
		int id=0;
		cout << n << " " << length << "\n";

	}
}


void f3(vector<string>&M, string &w){
	if(w=="a")cout << 0 << 0 << endl;
	vector< pair<string, vector<string> > > prev, curr;	
	vector<string>foo;	
	inverse(M, w, prev, foo);
	bool f=0;
	int n=1;
	while(prev.size()!=0){
		for(int i=0; i<prev.size(); i++){
			if (prev[i].first=="a"){
				f = 1;
			}
		}
		if (f) break;		
		curr.clear();
		for (int i=0; i<prev.size(); i++){
			inverse(M, prev[i].first, curr, prev[i].second);
		}
		prev = curr;
		n++;
			
	}
	if (f){
		cout << n << " ";
		unsigned long long int pre=INT_MAX;
		for (int i=0; i<prev.size(); i++){
			unsigned long long int curr_n=0;
			if (prev[i].first=="a"){
				for (int j=0; j<prev[i].second.size(); j++){
					for (int k=0; k<prev[i].second[j].size(); k++){
						curr_n += f1(M, j, prev[i].second[j][k]-'a');
					}
				}
				if (curr_n < pre) pre = curr_n;			
			}
		}
		cout << pre << endl;
	}
	else cout << -1 << "\n";
}

int main(){
int k;
cin >> k;
vector<string> M(k);
for (int i=0; i<k; i++)
	cin >> M[i];
int t;
cin >> t;
for(int i=0; i<t; i++){
	int c;
	cin >> c;
	if (c==0){
		unsigned long long int n; 
		cin >> n;
		cout << f1(M, n)<< "\n";
	}
	else if (c==1){
		unsigned long long int n;
		cin >> n;	
		cout << f2(M, n) << "\n";
	}
	else if (c==2){
		string x;
		cin >> x;
		f3(M, x);
	}
	
	else{
		string x;
		cin >> x;
		f4(M,x);
	}

}
}
