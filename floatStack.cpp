/* floatStack.cpp : 定义控制台应用程序的入口点。
* This is a Stack contains float numbers, we could get the mean/mode/sum in O(1) time complexity.
* Hmmmmmmmmmm, I am a good Chinglisher~~
* This GARBAGE is created by iHeadquarters 
* From 2013-03-08 to 2013-03-09 
*/

#include "stdafx.h"
#include <iostream>
using namespace std;

class Node
{
	public:
		Node()
		{
			value=0.0;
			next=NULL;
			nextMin=NULL;
			nextBucket=NULL;
		}

		float value;
		Node * next;
		Node * nextMin;
		Node * nextBucket;
};

class Buckets
{
	public:
		Buckets()
		{
			bucketValue=0.0;
			bucketsNumber=0;

			nextBuckets=NULL;
			nextBucket=NULL;
		}

		float bucketValue;
		int bucketsNumber;
		Buckets * nextBuckets;
		Node * nextBucket;
};


class Stack
{
	public:
		Stack()
		{
			top=new Node();
			top->next=NULL;
			top->nextMin=NULL;

			rope=new Buckets();
			rope->nextBuckets=NULL;

			sum=0.0;
			nodeNum=0;
		}

		Node * pop(){
			float popValue=0.0;
			Node * tmp;
			Node * tmpMin;
			Node * tmpBucket;
			//points to the Buckets whose value equals to the Node to be deleted
			Buckets * pBuckets;
			//whose nextBuckets is *pBuckets
			Buckets * p0Buckets;
			
			if(top->next==NULL)
				return NULL;

			popValue=top->next->value;
			sum-=popValue;
			nodeNum--;
			
			//pop out of the stack
			tmp=top->next;
			top->next=tmp->next;
			//nextMin in sorted linkedlist
			tmpMin=top;
			while(tmpMin->nextMin!=tmp)
			{
				tmpMin=tmpMin->nextMin;
			}
			tmpMin->nextMin=tmp->nextMin;

			//find the Buckets whose value equals to the Node to be deleted
			p0Buckets=rope;
			pBuckets=p0Buckets->nextBuckets;
			while((pBuckets!=NULL)&&((pBuckets->bucketValue)!=(tmp->value)))
			{
				p0Buckets=pBuckets;
				pBuckets=pBuckets->nextBuckets;
			}
			//delete the Bucket from the Buckets linkedlist
			tmpBucket=pBuckets->nextBucket;
			if(tmpBucket==tmp)
			{
				pBuckets->nextBucket=tmpBucket->nextBucket;
			}
			else
			{
				while((tmpBucket->nextBucket)!=tmp)
				{
					tmpBucket=tmpBucket->nextBucket;
				}
				tmpBucket->nextBucket=(tmpBucket->nextBucket)->nextBucket;
			}
			//count down the bucketsNumber
			(pBuckets->bucketsNumber)--;
			//release the pBuckets
			p0Buckets->nextBuckets=pBuckets->nextBuckets;
			if(pBuckets->bucketsNumber)//!=0
			{
				p0Buckets=rope;
				while(((p0Buckets->nextBuckets)!=NULL)&&((p0Buckets->nextBuckets)->bucketsNumber)>(pBuckets->bucketsNumber))
				{
					p0Buckets=p0Buckets->nextBuckets;
				}
				pBuckets->nextBuckets=p0Buckets->nextBuckets;
				p0Buckets->nextBuckets=pBuckets;
			}
			else//==0
			{
				delete pBuckets;//!
			}

			return tmp;
		}

		void push(float aValue)
		{
			Node * tmp;
			Buckets * pBuckets;
			Buckets * p0Buckets;
			Buckets * p1Buckets;

			//update the sum and nodeNumber
			sum+=aValue;
			nodeNum++;

			//create a new node
			Node *oneNew=new Node();
			oneNew->value=aValue;
			
			//push_top of stack
			oneNew->next=top->next;
			top->next=oneNew;
			//push_insert into the sorted linkedlist
			if(top->nextMin==NULL)
			{
				top->nextMin=oneNew;
				oneNew->nextMin=NULL;
			}
			else
			{
				tmp=top;
				while(tmp->nextMin&&tmp->nextMin->value<aValue)
				{
					tmp=tmp->nextMin;
				}
				oneNew->nextMin=tmp->nextMin;
				tmp->nextMin=oneNew;
			}

			//push_insert into the buckets
			pBuckets=rope;
			while((pBuckets->nextBuckets!=NULL)&&((pBuckets->nextBuckets)->bucketValue!=aValue))
			{
				pBuckets=pBuckets->nextBuckets;
			}
			if(pBuckets->nextBuckets==NULL)
			{
				//insert a new buckets into the end of Buckets linkedlist
				Buckets * oneNewBuckets=new Buckets();
				oneNewBuckets->bucketsNumber=1;
				oneNewBuckets->bucketValue=aValue;
				oneNewBuckets->nextBuckets=pBuckets->nextBuckets;
				pBuckets->nextBuckets=oneNewBuckets;

				oneNewBuckets->nextBucket=oneNew;
				oneNew->nextBucket=NULL;
			}
			else
			{
				//p0Buckets points to the Buckets which value equals aValue
				p0Buckets=pBuckets->nextBuckets;
				(p0Buckets->bucketsNumber)++;
				oneNew->nextBucket=p0Buckets->nextBucket;
				p0Buckets->nextBucket=oneNew;

				pBuckets->nextBuckets=p0Buckets->nextBuckets;

				p1Buckets=rope;
				while(((p1Buckets->nextBuckets)!=NULL)&&(((p1Buckets->nextBuckets)->bucketsNumber)>(p0Buckets->bucketsNumber)))
				{
					p1Buckets=p1Buckets->nextBuckets;
				}

				p0Buckets->nextBuckets=p1Buckets->nextBuckets;
				p1Buckets->nextBuckets=p0Buckets;
			}
		}

		int getNodeNum()
		{
			return nodeNum;
		}

		float getMean()
		{
			return sum/getNodeNum();
		}
				
		float getMin()
		{
			return top->nextMin->value;
		}

		float getMode()
		{
			return (rope->nextBuckets)->bucketValue;
		}

		void show(){
			Node * tmp=top;
			while(tmp->next!=NULL){
				tmp=tmp->next;
				cout<<tmp->value<<" ";
			}
			cout<<endl;
		}

		void showSorted(){
			Node *tmp=top;
			while(tmp->nextMin){
				tmp=tmp->nextMin;
				cout<<tmp->value<<" ";
			}
			cout<<endl;
		}

		void showBuckets()
		{
			Buckets * pBuckets=rope;
			while(pBuckets->nextBuckets!=NULL)
			{
				pBuckets=pBuckets->nextBuckets;
				cout<<">"<<pBuckets->bucketValue<<" : "<<pBuckets->bucketsNumber<<endl;
			}
		}

		private:
			Node * top;
			Buckets * rope;//head of the Buckets
			float sum;
			int nodeNum;
};

int main(int argc, _TCHAR* argv[])
{
	float n;
	Stack stack=Stack::Stack();

	//use a array to test the operations
	float a[15]={3,5,2,4,5,3,5,4,2,3,1,5,5,4,4};
	for(int i=0;i<15;i++)
	{
		stack.push(a[i]);
	}

	/*while(cin>>n,n!=0.0)
	{
		stack.push(n);
	}*/
	cout<<"there are "<<stack.getNodeNum()<<" nodes, mean value is "<<stack.getMean()<<endl;//sum of the 15 numbers should be 55,mean should be 3.667
	stack.show();
	cout<<"sorted stack: ";
	stack.showSorted();
	cout<<"stack mode: \n";
	stack.showBuckets();

	cout<<"after poped out:"<<stack.pop()->value<<endl;
	cout<<"after poped out:"<<stack.pop()->value<<endl;
	cout<<"there are "<<stack.getNodeNum()<<" nodes, mean value is "<<stack.getMean()<<endl;//sum of the 13 numbers should be 47,mean should be 3.615
	stack.show();
	cout<<"sorted stack: ";
	stack.showSorted();
	cout<<"stack mode: \n";
	stack.showBuckets();

	//test poping out all of the nodes in the stack
	for(int i=0;i<15;i++){
		Node * popOut=stack.pop();
		if(popOut)
		{
			cout<<"Test pop "<<popOut->value<<endl;
		}
		else
		{
			cout<<"Test pop NULL"<<endl;
		}
	}

	return 0;
}

//

