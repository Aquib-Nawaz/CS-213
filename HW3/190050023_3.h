#include<bits/stdc++.h>
using namespace std;

class quad_tree{

	pair<int, int> topLeft;
	pair<int, int> botRight;
	int level;
	int data; 
    //public:  
    // Children of this tree 
	quad_tree *topLeftTree; 
	quad_tree *topRightTree; 
	quad_tree *botLeftTree; 
	quad_tree *botRightTree; 

	void delete_child(){
		delete topLeftTree;
		delete topRightTree;
		delete botLeftTree;
		delete botRightTree;
	}

	void set_null(){
		topLeftTree=NULL;	
		topRightTree=NULL;
		botLeftTree=NULL; 
	        botRightTree=NULL;
	}

	void reduce(){
		if (topLeftTree==NULL) return;
		if (topLeftTree->topLeftTree == NULL && topRightTree->topLeftTree == NULL && botLeftTree->topLeftTree == NULL && botRightTree->topLeftTree == NULL){
			int d = topLeftTree->data;
			if (d == topRightTree->data && d == botLeftTree->data && d == botRightTree->data){
				data = d;
				this->delete_child();
				this->set_null();
				return;		
			}
		}
	}

	void squeeze(long int &c_0, long int &c_1, long int &ref){
		if (c_0 > ref || c_1 >=ref) return;
		long int t = 1<<2*level;
		if(topLeftTree==NULL) {
			if(data) c_1 += t;
			else c_0 += t; 
			return;
		}
		topLeftTree->squeeze(c_0, c_1, ref);
		topRightTree->squeeze(c_0, c_1, ref);
		botLeftTree->squeeze(c_0, c_1, ref);
		botRightTree->squeeze(c_0, c_1, ref);
		return;	
	}
	//void set(int x1, int y1, int x2, int y2, int b);
	void exhelp(quad_tree *Q, int x, int y, int m){
		if(botRight.first < x || botRight.second < y || topLeft.first >= x+1<<m || topLeft.second >= y+1<<m) return;
		if(topLeftTree == NULL){
			int t=1<<m;
			//cout << max(topLeft.first, x)-x <<max(topLeft.second,y)-y<<min(botRight.first, x-1+t)-x<<min(botRight.second, y-1+t)-y << endl;
			Q->set(max(topLeft.first, x)-x, max(topLeft.second,y)-y, min(botRight.first, x-1+t)-x, min(botRight.second, y-1+t)-y, data);
			return;
		}
		topLeftTree->exhelp(Q, x, y,  m);
		topRightTree->exhelp(Q, x, y, m);
		botLeftTree->exhelp(Q, x, y, m);
		botRightTree->exhelp(Q, x, y, m);
	}

    public:   	
	
	quad_tree(int n){
		data=0;
		level = n;
		int s=1<<n;
		topLeft = make_pair(0, 0);
		botRight = make_pair(s-1, s-1);
		this->set_null();
	}
	
	~quad_tree() {
		this->delete_child();	
	}

	quad_tree(quad_tree const &Q){
		level = Q.level;
		topLeft = Q.topLeft;
	 	botRight = Q.botRight; 
		if(Q.topLeftTree==NULL){
			data = Q.data;
			this->set_null();	
		}
		else{
			topLeftTree = new quad_tree(*Q.topLeftTree);
			topRightTree = new quad_tree(*Q.topRightTree);
			botLeftTree = new quad_tree(*Q.botLeftTree);
			botRightTree = new quad_tree(*Q.botRightTree);
		}
	}
	
	void set(int x1, int y1, int x2, int y2, int b) {
		//cout << x1 <<" " << y1 << " " << x2 << " "<< y2 << "\n";
		int t = 1<<(level-1);
		if(x1>x2 || y1>y2) return;
		if(topLeft.first==x1 && topLeft.second==y1 && botRight.first==x2 && botRight.second==y2){
			data=b;
			this->delete_child();
			this->set_null();
			return;			
		}
		if(topLeftTree==NULL){
			if(data==b)return;
			else{
				topLeftTree = new quad_tree(level-1);
				topLeftTree->topLeft = make_pair(topLeft.first, topLeft.second);
				topLeftTree->botRight = make_pair(topLeft.first+t-1, topLeft.second+t-1);
				topLeftTree->data = data;
				topRightTree = new quad_tree(level-1);
				topRightTree->topLeft = make_pair(topLeft.first, topLeft.second+t);
				topRightTree->botRight = make_pair(topLeft.first+t-1, botRight.second); 
				topRightTree->data = data;
				botLeftTree = new quad_tree(level-1);
				botLeftTree->topLeft = make_pair(topLeft.first+t, topLeft.second);
				botLeftTree->botRight = make_pair(botRight.first, topLeft.second+t-1);
				botLeftTree->data = data;
				botRightTree = new quad_tree(level-1);
				botRightTree->topLeft = make_pair(topLeft.first+t, topLeft.second+t);
				botRightTree->botRight = make_pair(botRight.first, botRight.second);
				botRightTree->data = data;
					
			}			
		}
		topLeftTree->set(x1, y1, min(x2, topLeft.first+t-1), min(y2, topLeft.second+t-1), b);
		topRightTree->set(x1, max(topLeft.second+t, y1), min(x2, topLeft.first+t-1), y2, b);
		botLeftTree->set(max(topLeft.first+t, x1), y1, x2, min(y2, topLeft.second+t-1), b);
		botRightTree->set(max(topLeft.first+t, x1), max(topLeft.second+t, y1), x2, y2, b);
		this->reduce();	
	}

	int get(int x1, int y1) const {
		if (topLeftTree == NULL) return data;
		int t=1<<(level-1);
		if(x1 < topLeft.first+t){
			if (y1 < topLeft.second+t) return topLeftTree->get(x1, y1);
			else return topRightTree->get(x1, y1);
		}
		else{
			if (y1 < topLeft.second+t) return botLeftTree->get(x1, y1);
			else return botRightTree->get(x1, y1);		
		}
	}

	int size () const{return level;}

	void overlap(quad_tree const &Q){
		if(topLeftTree != NULL){
			if (Q.topLeftTree != NULL) {
				topLeftTree->overlap(*Q.topLeftTree);
				topRightTree->overlap(*Q.topRightTree);
				botLeftTree->overlap(*Q.botLeftTree);
				botRightTree->overlap(*Q.botRightTree);
				this->reduce();
				return;
			}
			else {
				if (Q.data==0) return;
				data=1;
				this->delete_child();
				this->set_null();
				return;		
			}
		}
		if(data==1)return;
		if (Q.topLeftTree != NULL){
			topLeftTree = new quad_tree(*Q.topLeftTree);	
			topRightTree = new quad_tree(*Q.topRightTree);
			botLeftTree = new quad_tree(*Q.botLeftTree); 
			botRightTree = new quad_tree(*Q.botRightTree);
			return;
		}
		data = Q.data;
		return;
	}	

	void intersect(quad_tree &Q){
		if(topLeftTree != NULL){
			if (Q.topLeftTree != NULL) {
				topLeftTree->intersect(*Q.topLeftTree);
				topRightTree->intersect(*Q.topRightTree);
				botLeftTree->intersect(*Q.botLeftTree);
				botRightTree->intersect(*Q.botRightTree);
				this->reduce();
				return;
			}
			else {
				if (Q.data==1) return;
				data=0;
				this->delete_child();
				this->set_null();
				return;		
			}
		}
		if(data==0)return;
		if (Q.topLeftTree != NULL){
			topLeftTree = new quad_tree(*Q.topLeftTree);	
			topRightTree = new quad_tree(*Q.topRightTree);
			botLeftTree = new quad_tree(*Q.botLeftTree); 
			botRightTree = new quad_tree(*Q.botRightTree);
			return;
		}
		data = Q.data;
		return;
	}

	void complement() {

		if (topLeftTree==NULL) {data = !data; return;}
		topLeftTree->complement();
		topRightTree->complement();
		botLeftTree->complement();
		botRightTree->complement();
		return;
	}

	void resize(int m) {
		if (level==m)return;
		if (m!=0){
			level = m;
			int t = 1<<level;
			botRight = make_pair(topLeft.first+t-1, topLeft.second+t-1);
			if(topLeftTree == NULL) return;
			topLeftTree->topLeft = topLeft;
			topRightTree->topLeft = make_pair(topLeft.first, topLeft.second+t/2);
			botLeftTree->topLeft = make_pair(topLeft.first+t/2, topLeft.second);	
			botRightTree->topLeft = make_pair(topLeft.first+t/2, topLeft.second+t/2);
			topLeftTree->resize(m-1);
			topRightTree->resize(m-1);
			botLeftTree->resize(m-1);
			botRightTree->resize(m-1);
			return;
		}
	        long int ref = 1<<(2*level-1), c_0=0, c_1=0;
		this->squeeze(c_0, c_1, ref);
		if(c_1>=ref) data=1;
		else data=0;
		level = 0;
		botRight=topLeft;
		this->delete_child();
		this->set_null();
		return;	
	}

	void extract(int x1, int y1, int m){
		quad_tree *Q = new quad_tree(m);
		this->exhelp(Q, x1, y1, m);	
		this->delete_child();
		level = m;
		topLeft = Q->topLeft;
		botRight = Q->botRight;
		if (Q->topLeftTree == NULL){this->set_null(), data = Q->data;}
		else {topLeftTree=new quad_tree(*(Q->topLeftTree)); topRightTree = new quad_tree(*(Q->topRightTree)); botLeftTree = new quad_tree(*(Q->botLeftTree)); botRightTree = new quad_tree(*(Q->botRightTree));}
		
	}
/*
	void print(){
		for (int i=topLeft.first; i<=botRight.first; i++){
			for(int j=topLeft.second; j<=botRight.second; j++)cout << this->get(i,j) << " ";
			cout << "\n";			
		}
	}
*/
};


