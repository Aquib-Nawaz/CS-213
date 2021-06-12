#include<bits/stdc++.h>
using namespace std;

long long int gcd(long long int a, long long int b){
	for (;;)
    {
        if (a == 0) return b;
        b %= a;
        if (b == 0) return a;
        a %= b;
    }
}

long long int LCM(long long int a, long long int b){
	long long int gc = gcd(a, b);
	return (a/gc)*b;
}

class permutation{

private:

	int *s, *value;


	permutation identity() const{
		int A[this->size()];
		for (int i=0; i<this->size(); i++)A[i]=i;
		return permutation(this->size(), A);

	}

	vector< vector<int> >disjoint_cycles()const{
		int siz=this->size();
		vector<vector<int>> Cycles;
		int start=0, *A=this->to_array();
		vector<int> cycle_made;
		while(cycle_made.size()!=siz){
			vector<int> cycle;
			int j=start;
			while (true) {
				cycle.push_back(j);
				cycle_made.push_back(j);
				j=A[j];
				if (start==j) break;
				
			}
			for (int i = 0; i < siz; ++i)
			{
				if (find(cycle_made.begin(), cycle_made.end(), i)==cycle_made.end()){
					start=i;
					break;
				}

			}
			if (cycle.size()!=1) Cycles.push_back(cycle);
		}
		return Cycles;

	}


	permutation(vector< vector<int> > Cycles, int n){
		s = new int;
		*s = n;
		value = new int[n];
		for (int i=0; i<n; i++) value[i]=i;
		for(int i=0; i<Cycles.size(); i++)
			for(int j=0; j<Cycles[i].size(); j++){
				if (j==Cycles[i].size()-1) value[Cycles[i][j]]=Cycles[i][0];
				else value[Cycles[i][j]]=Cycles[i][j+1];
			}
		
	}

public:
	permutation (int n, int A[]) {
		s = new int;
		*s = n;
		value = new int[n];
		for (int i=0; i<n; i++) value[i] = A[i];		 
	}

    ~permutation () {
    	        delete s;
    	        delete [] value;
	}

	permutation (permutation const &q) {
		s = new int;
		*s = q.size();
		value = new int[size()];
		for (int i=0; i<size(); i++) value[i] = q.value[i];	
	}

	permutation const operator=(permutation const &q){
		s = new int;
		*s = q.size();
		for(int i=0; i<*s; i++)
			value[i] = q.value[i];
		return *this;	
	}

	int size () const{
		return *(this->s);
	}

	int* to_array() const{
		int siz=this->size();
		int* A = new int[siz];
		for (int i=0; i<siz; i++) A[i] = this->value[i];
		return A;
	}

	void print () const{
		for (int i=0; i<this->size(); i++) cout << this->value[i] << " ";
		cout << "\n";
	}	

	permutation const operator-() const{
		int siz=this->size();
		int* A = new int[siz];
		for (int i=0; i<siz; i++) A[this->value[i]] = i;
		return permutation(siz, A);
	}

	permutation const operator *(permutation const &q) const{
		int siz=this->size();
		int *A = new int[siz];
		for (int i=0; i<siz; i++) A[i] = this->value[q.value[i]];
		return permutation(siz, A);
	}
	permutation const operator^(long long int i) const
	{
		int arr[61];
		for (int j = 60;j>=0;j--)
		{
			long long int k = pow(2,j);			
			arr[j]=i/k;
			i=i%k;
		}
		permutation res((*this)*(-(*this)));
		permutation q(*this);
		for (int j=0;j<=60;j++)
		{
			if(arr[j]==1)
				res=res*q;
			q=q*q;
		}		
		return res;
	}
	bool is_power(permutation const &q) const{
		
		map<int, int> len_rem;
		vector<vector<int>> Cycles = q.disjoint_cycles();
		int *A = this->to_array();
		for(int i=0; i<Cycles.size(); i++){
			int len = Cycles[i].size(), rem=-1;       //finding rem for first
			for(int j=0; j<len; j++){					//element
				if (A[Cycles[i][0]]==Cycles[i][j]){
					rem = j; break;
				}
			}
			if(rem==-1) return false;				//if such rem is not found
			for (int j=0; j<len; j++){
				if (A[Cycles[i][j]]!=Cycles[i][(j+rem)%len]) //if rem doesn't
					return false;							//satisfy for all elements
			}

			if(len_rem.find(len)!=len_rem.end()){
				if(len_rem[len]!=rem) return false; //if rem corresponding to two
			}										//same len cycle is not same
			else{
				len_rem.insert({len, rem});
			}
		}
		long long int lcm=len_rem.begin()->first, remainder=len_rem.begin()->second;
		map<int, int>::iterator it;
		for(it = len_rem.begin(); it!=len_rem.end(); it++){
			long long int new_lcm = LCM(it->first, lcm);
			while(remainder%(it->first) != it->second){
				remainder+=lcm;
				if(remainder > new_lcm) return 0;
			}
			lcm = new_lcm;
		}
		return true;
	}


	int log(permutation const &q) const{
		if(!this->is_power(q)) return 0;

		map<int, int> len_rem;
		vector<vector<int>> Cycles = q.disjoint_cycles();
		int *A = this->to_array(); bool is_identity = true;
		for(int i=0; i<this->size(); i++){
			if (A[i]!=i) {is_identity = false; break;}

		}
		if (is_identity) return 0;		
		for(int i=0; i<Cycles.size(); i++){
			int len=Cycles[i].size();
			if (len_rem.find(len)==len_rem.end()){
				
				int rem;
				for(rem=0; rem<len; rem++){					
					if (A[Cycles[i][0]]==Cycles[i][rem]){
				   		break;
					}
				}
				len_rem.insert({len, rem});	
			}
		}
		long long int lcm=len_rem.begin()->first, remainder=len_rem.begin()->second;
		map<int, int>::iterator it;
		for(it = len_rem.begin(); it!=len_rem.end(); it++){
			long long int new_lcm = LCM(it->first, lcm);
			while(remainder%(it->first) != it->second){
				remainder+=lcm;
				if(remainder > new_lcm) return 0;
			}
			lcm = new_lcm;
		}
		
		return remainder%(1000000007);
	}
		
	permutation const square_root () const{
		permutation res(*s, value);
		vector<vector<int>> Cycles=res.disjoint_cycles();
		
                 
		vector<vector<int>> v; 
		for (int i=0; i<Cycles.size(); i++){
			int S = Cycles[i].size();
			if (S%2==0){
				bool find=false;
				for (int j=i+1; j<Cycles.size(); j++){
					if(Cycles[j].size()==S){
						find = true;
						vector<int> cycle;
						for(int k=0; k<S; k++){
							cycle.push_back(Cycles[i][k]);
							cycle.push_back(Cycles[j][k]);
							
						}
						Cycles.erase(Cycles.begin()+j);
						Cycles.erase(Cycles.begin()+i);
						i--;
						v.push_back(cycle);
						break;
					}
				}			
				if (!find) return this->identity();			
			}
			else {
				vector<int>cycle;
				for(int k=0;k<(S-1)/2;k++)
				{
					cycle.push_back(Cycles[i][k]);
					cycle.push_back(Cycles[i][k+(S+1)/2]);
					
				}
				cycle.push_back(Cycles[i][(S-1)/2]);
			
				v.push_back(cycle);
			}
		}
		
		return permutation(v, this->size());
		
	}

};
	


