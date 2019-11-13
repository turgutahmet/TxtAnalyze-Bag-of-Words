//author: Ahmet TURGUT 150117046

#include <stdio.h> 
#include <dirent.h> 
#include <stdlib.h>
#include <string.h>
#include <Math.h>

typedef struct info{//node's information
	char topic[76];
	int txt;
	struct info *next;
}info;

typedef struct MLL{//master linked list node
	char word[76];
	int howManyWord[3];
	struct info *infoWord;
	struct MLL *next;
	struct order *first;
	struct order *second;
	struct order *third;
}MLL;

typedef struct order{//first,second and third order's struct
	struct MLL *node;
	struct order *next;
}order;

typedef struct wordLL{ //this goal is create list to idf and most frequent words
	char word[76];
	int howMany;
	double idf;
	struct MLL *node;
	struct wordLL *next;
}wordLL;

	char topicName[3][76];//i use for topic name
	int sum[3];			  //i use for sum of fie in folder
	
void counterCalculator(char *topic,MLL *master){//counter how many words in one folder
			if(strcmp(topic,topicName[0])==0){
				master->howManyWord[0]=1;
				master->howManyWord[1]=0;
				master->howManyWord[2]=0;
			}else if(strcmp(topic,topicName[1])==0){
				master->howManyWord[1]=1;
				master->howManyWord[0]=0;
				master->howManyWord[2]=0;
			}else if(strcmp(topic,topicName[2])==0){
				master->howManyWord[2]=1;
				master->howManyWord[1]=0;
				master->howManyWord[0]=0;
			}
}

int findNumber(char* name){ //find the number of txt in folder
	struct dirent *de;
	
	char nameX[100] ;
	strcpy(nameX,"dataset\\\\");
	strcat(nameX,name);
	
	DIR *dr = opendir(nameX); 
	
	if (dr == NULL) { 
		printf("Error" ); 
		return 0; } 

	int i=0;
	while ((de = readdir(dr)) != NULL) {i++;}
	i=i-2;			
	closedir(dr);
	 return i;
}

void printFile(char* topic,int numberOfFile,MLL **masterLinkedList) {//create a MasterLinkedlist (bag of words) with information 
	MLL *masterHead;
	MLL *iterMaster;
	*masterLinkedList=masterHead;
	char nameX[150] ;
	char num[sizeof(int)*8+1];
	itoa(numberOfFile,num,10);
	strcpy(nameX,"dataset\\\\");
	strcat(nameX,topic);
	strcat(nameX,"\\\\");
	strcat(nameX,num);
	strcat(nameX,".txt");
	
	FILE *fp = fopen(nameX,"r");
 
    char buf[76];
 
        while( fscanf(fp, "%s", buf) != EOF )
        {
        	
           MLL *control=masterHead;
           int tf=1;
           
           while(control!=NULL){
	           	if(!strcmp(control->word,buf)){
				tf=0;
				if(strcmp(topic,topicName[0])==0){
					control->howManyWord[0]=control->howManyWord[0]+1;
				}else if(strcmp(topic,topicName[1])==0){
					control->howManyWord[1]=control->howManyWord[1]+1;
				}else if(strcmp(topic,topicName[2])==0){
					control->howManyWord[2]=control->howManyWord[2]+1;
				}
				
				info *infoControl=control->infoWord;
				if(infoControl->next==NULL){
					infoControl->next=(info*)malloc(sizeof(info));
					strcpy(infoControl->next->topic,topic);
					infoControl->next->txt=numberOfFile;
					infoControl->next->next=NULL;
				}else{
					while(infoControl->next->next!=NULL){
						infoControl=infoControl->next;
					}
					infoControl->next->next=(info*)malloc(sizeof(info));
					strcpy(infoControl->next->next->topic,topic);
					infoControl->next->next->txt=numberOfFile;
					infoControl->next->next->next=NULL;
				}
			}
		control=control->next;
		}
		   
		if(tf==1){
		
			if(masterHead==NULL){
				masterHead=(MLL*)malloc(sizeof(MLL));
				strcpy(masterHead->word,buf);
				masterHead->next=NULL;
				iterMaster=masterHead;
				
				counterCalculator(topic,masterHead);
				
				masterHead->infoWord=(info*)malloc(sizeof(info));
				strcpy(masterHead->infoWord->topic,topic);
				masterHead->infoWord->txt=numberOfFile;
				masterHead->infoWord->next=NULL;
					
			}else {
				
				iterMaster->next=(MLL*)malloc(sizeof(MLL));
				iterMaster=iterMaster->next;
				strcpy(iterMaster->word,buf);
				iterMaster->next=NULL;
				
				counterCalculator(topic,iterMaster);
				
				iterMaster->infoWord=(info*)malloc(sizeof(info));
				strcpy(iterMaster->infoWord->topic,topic);
				iterMaster->infoWord->txt=numberOfFile;
				iterMaster->infoWord->next=NULL;
			}
		}
        }
}
int boolean(info *iControl,info *iControl2){ //call in the firstOrderFind function.
	info *infoControl,*infoControl2;
	infoControl=iControl;
	
	 while(infoControl!=NULL){
	 	infoControl2=iControl2;
	 	
	 	while(infoControl2!=NULL){
	 		if(strcmp(infoControl->topic,infoControl2->topic) == 0 && (infoControl->txt==infoControl2->txt) == 1){
	 			return 1;
			 }
			 
			 infoControl2=infoControl2->next;
		 }
		 infoControl=infoControl->next;
	 }
	return 0;
}
void firstOrderFind(MLL **masterLinkedList){//create a firstOrder List
	MLL *masterHead;
	MLL *tempMaster;
	MLL *iterMaster;
	order *firstHead;
	masterHead=*masterLinkedList;
	tempMaster=masterHead;
	
	while(tempMaster!=NULL){
		iterMaster=masterHead;
		
		while(iterMaster!=NULL){
		
			if(boolean(tempMaster->infoWord,iterMaster->infoWord) == 1 && strcmp(tempMaster->word,iterMaster->word) != 0){
				if(tempMaster->first==NULL){
					tempMaster->first=(order*)malloc(sizeof(order));
					tempMaster->first->node=iterMaster;
					tempMaster->first->next=NULL;
					firstHead=tempMaster->first;
					
				}else {
					while(firstHead->next!=NULL){
						firstHead=firstHead->next;
					}
					firstHead->next=(order*)malloc(sizeof(order));
					firstHead->next->node=iterMaster;
					firstHead->next->next=NULL;
				}
			}
			
			iterMaster=iterMaster->next;
		}
		tempMaster=tempMaster->next;
	}
}
void secondOrderFind(MLL **masterLinkedList){ //create a secondOrder List
	MLL *masterHead;
	MLL *tempMaster;
	order *firstHead,*firstHead2,*firstHead3;
	order *secondHead;
	masterHead=*masterLinkedList;
	tempMaster=masterHead;
	int k=1,c=1;
	while(tempMaster!=NULL){
			firstHead=tempMaster->first;
			
			while(firstHead!=NULL){
				
				firstHead2=firstHead->node->first;
				
				while(firstHead2!=NULL){
					
					firstHead3=tempMaster->first;
					k=1;
					while(firstHead3!=NULL){
						
						if(strcmp(firstHead3->node->word,firstHead2->node->word) == 0 || strcmp(tempMaster->word,firstHead2->node->word) == 0){
							k=0;break;
						}
						firstHead3=firstHead3->next;
					}
					
					if(k==1){
						if(tempMaster->second==NULL){
							tempMaster->second=(order*)malloc(sizeof(order));
							tempMaster->second->node=firstHead2->node;
							tempMaster->second->next=NULL;
						
						}else{
							
							c=1;
							secondHead=tempMaster->second;
							while(secondHead->next!=NULL){
								if(strcmp(secondHead->node->word,firstHead2->node->word)==0){
									c=0;
								}
								secondHead=secondHead->next;
								if(strcmp(secondHead->node->word,firstHead2->node->word)==0){
									c=0;
								}
							
							}
							if(c==1){
								secondHead->next=(order*)malloc(sizeof(order));
							secondHead->next->node=firstHead2->node;
							secondHead->next->next=NULL;
							}
							
						}
					}
					firstHead2=firstHead2->next;
				}
				
				firstHead=firstHead->next;
				
			}tempMaster=tempMaster->next;
	}
}
void thirdOrderFind(MLL **masterLinkedList){//create a thirdOrder List
	MLL *masterHead;
	MLL *tempMaster;
	order *firstHead;
	order *secondHead,*secondHead2,*secondHead3;
	order *thirdHead;
	masterHead=*masterLinkedList;
	tempMaster=masterHead;
		
	int k=1,c=1,d=1;
	while(tempMaster!=NULL){
			
			secondHead=tempMaster->second;
			
			while(secondHead!=NULL){
				
				secondHead2=secondHead->node->second;
				
				while(secondHead2!=NULL){
					
					secondHead3=tempMaster->second;
					k=1;
					while(secondHead3!=NULL){
						
						if(strcmp(secondHead3->node->word,secondHead2->node->word) == 0 || strcmp(tempMaster->word,secondHead2->node->word) == 0){
							k=0;break;
						}
						secondHead3=secondHead3->next;
					}
					firstHead=tempMaster->first;
					c=1;
					while(firstHead!=NULL){
						
						if(strcmp(firstHead->node->word,secondHead2->node->word)==0){
							c=0;break;
						}
						firstHead=firstHead->next;
					}
					if(k==1&&c==1){
							
						if(tempMaster->third==NULL){
							tempMaster->third=(order*)malloc(sizeof(order));
							tempMaster->third->node=secondHead2->node;
							tempMaster->third->next=NULL;
							
						}else{
							d=1;
							thirdHead=tempMaster->third;
							while(thirdHead->next!=NULL){
								if(strcmp(thirdHead->node->word,secondHead2->node->word)==0){
									d=0;
								}
								thirdHead=thirdHead->next;
								if(strcmp(thirdHead->node->word,secondHead2->node->word)==0){
									d=0;
								}
								
							}
							if(d==1){
							thirdHead->next=(order*)malloc(sizeof(order));
							thirdHead->next->node=secondHead2->node;
							thirdHead->next->next=NULL;
							}
							
						}
					
					}
				
					secondHead2=secondHead2->next;
				}
				
				secondHead=secondHead->next;
				
			}tempMaster=tempMaster->next;
	}
}
int findTopicName(){ //this function goal is find the topic name and write in topicName array
	struct dirent *de; 
  	int k=0;
    DIR *dr = opendir("dataset////"); 
  
    if (dr == NULL)  
    { 
        printf("Error" ); 
        return 0; 
    } 
  
    while ((de = readdir(dr)) != NULL){
    	if(strcmp(de->d_name,".") == 0 ||strcmp(de->d_name,"..") == 0){
    		
		}else{
			strcpy(topicName[k],de->d_name);k++;
		}	
	}
    closedir(dr); 
}
double countIDF(wordLL *control,int k){//calculate IDF // hint (CSE225_fall_2019_Project.pdf): IDF (t) =loge (N/n)
	
	info *temp=control->node->infoWord;
	int count=0;int contr=0;
	while(temp!=NULL){
		
		if(strcmp(topicName[k],temp->topic)==0 && temp->txt!=contr ){
			contr=temp->txt;count++;
		}
		temp=temp->next;
	}
	
	double result,IDF;
	
	result=sum[k]/(double)count;
	IDF=log(result);
	
	return IDF;	
}

void howManyWord(MLL **masterLinkedList,wordLL **sub,int k){ //create word linked list for most frequency words
	MLL *masterHead;
	masterHead=*masterLinkedList;
	wordLL *control;
	control=*sub;
	while(masterHead!=NULL){
		if(masterHead->howManyWord[k]!=0){
			if(control==NULL){
			control=(wordLL*)malloc(sizeof(wordLL));
			control->next=NULL;
			control->howMany=masterHead->howManyWord[k];
			control->node=masterHead;
			strcpy(control->word,masterHead->word);
			control->idf=countIDF(control,k);
			masterHead=masterHead->next; continue;
			}
		if(control->howMany < masterHead->howManyWord[k]){
			
			wordLL *temp=(wordLL*)malloc(sizeof(wordLL));
			temp->howMany=masterHead->howManyWord[k];
			strcpy(temp->word,masterHead->word);
			temp->node=masterHead;
			temp->idf=countIDF(temp,k);
			temp->next=control; control=temp;masterHead=masterHead->next; continue;
			
		}
		wordLL *iter=control;
		while(iter->next!=NULL && iter->next->howMany > masterHead->howManyWord[k]){
		
			iter=iter->next;
		}
		wordLL *temp=(wordLL*)malloc(sizeof(wordLL));
		temp->next=iter->next;
		iter->next=temp;
		temp->howMany=masterHead->howManyWord[k];
		strcpy(temp->word,masterHead->word);
		temp->node=masterHead;
		temp->idf=countIDF(temp,k);
		masterHead=masterHead->next;continue;
		}
		masterHead=masterHead->next;
			
	}*sub=control;
	

}

int printWordsEachCategory(wordLL **list,wordLL **list2,wordLL **list3,FILE **out){//print the most frquency 5 words in for each topic
	wordLL *control,*control2,*control3;
	control=*list;control2=*list2;control3=*list3;
	FILE *output=*out;
	
	int k=0;
	fprintf(output,"Most frequent 5 words\n");
	fprintf(output,"%s    : \n",topicName[0]);
	while(control!=NULL){
		fprintf(output,"[%s , %d] \n",control->word,control->howMany);
		control=control->next;k++;if(k==5)break;
	}fprintf(output,"\n");
	fprintf(output,"%s    : \n",topicName[1]);k=0;
	while(control2!=NULL){
		fprintf(output,"[%s , %d] \n",control2->word,control2->howMany);
		control2=control2->next;k++;if(k==5)break;
	}fprintf(output,"\n");
	fprintf(output,"%s    : \n",topicName[2]);k=0;
	while(control3!=NULL){
		fprintf(output,"[%s , %d] \n",control3->word,control3->howMany);
		control3=control3->next;k++;if(k==5)break;
	}fprintf(output,"\n");
	control=*list;control2=*list2;control3=*list3;
	
	
	return 1;
}
int printWordsEachCategoryIDF(wordLL **list,wordLL **list2,wordLL **list3,FILE **out){//print the most idf 5 words in for each topic  
	wordLL *control,*control2,*control3;
	control=*list;control2=*list2;control3=*list3;
	FILE *output=*out;
	int k=0;
	fprintf(output,"Most frequency 5 words\n");
	fprintf(output,"%s    : \n",topicName[0]);
	while(control!=NULL){
		fprintf(output,"[%s , %f] \n",control->word,control->idf);
		control=control->next;k++;if(k==5)break;
	}fprintf(output,"\n");
	fprintf(output,"%s    : \n",topicName[1]);k=0;
	while(control2!=NULL){
		fprintf(output,"[%s , %f] \n",control2->word,control2->idf);
		control2=control2->next;k++;if(k==5)break;
	}fprintf(output,"\n");
	fprintf(output,"%s    : \n",topicName[2]);k=0;
	while(control3!=NULL){
		fprintf(output,"[%s , %f] \n",control3->word,control3->idf);
		control3=control3->next;k++;if(k==5)break;
	}fprintf(output,"\n");
	control=*list;control2=*list2;control3=*list3;
	
	
	return 1;
}
void exchange(wordLL *a, wordLL *b) { //call in the sort method for swap
   	double temp=a->idf;
   	char x[76];
   	strcpy(x,a->word);
   	MLL *iter=a->node;
   	a->idf=b->idf;
   	a->node =b->node;
   	strcpy(a->word,b->word);
   	
   	b->idf=temp;
   	strcpy(b->word,x);
   	b->node=iter;
}
void sort(wordLL **start){ //Function goal is sorting to word Linked List with idf value
   int temp, i; 
    wordLL *p; 
    wordLL *r = NULL; 
    
    do{ 
        temp = 0; 
        p = *start; 
        while (p->next != r) { 
            if (p->idf < p->next->idf) {  
                exchange(p, p->next); 
                temp = 1; 
            } 
            p = p->next; 
        } 
        r = p; 
    }while (temp);
} 
  
int main(){
	printf("Program is running.\n");
	MLL *masterLinkedList=NULL;
	MLL *tempMaster;
	order *thirdHead;
	order *secondHead;
	order *firstHead;
	
	findTopicName(); //copy topic name in topicName[] array

	sum[0]=findNumber(topicName[0]); //calculate the how many txt in folder? 
	sum[1]=findNumber(topicName[1]);
	sum[2]=findNumber(topicName[2]);
	
	for(int k=1;k<=sum[0];k++){
		printFile(topicName[0],k,&masterLinkedList);//create masterLinkedList for 1.category
	}
	for(int k=1;k<=sum[1];k++){
		printFile(topicName[1],k,&masterLinkedList);//create masterLinkedList for 2.category
	}
	for(int k=1;k<=sum[2];k++){
		printFile(topicName[2],k,&masterLinkedList);//create masterLinkedList for 3.category
	}
	printf("MasterLinkedList was created\n");	
	tempMaster=masterLinkedList;
	while(tempMaster!=NULL){//Need to do NULL in masterLinkedList word's head
		
		tempMaster->first=NULL;
		tempMaster->second=NULL;
		tempMaster->third=NULL;
		tempMaster=tempMaster->next;
	}
	wordLL *firstSub=NULL;//create wordLL this goal is create a word linked List for most frequecny words
	wordLL *secondSub=NULL;
	wordLL *thirdSub=NULL;
	
	howManyWord(&masterLinkedList,&firstSub,0);//Calculate the Most frequent 5 words for each category
	howManyWord(&masterLinkedList,&secondSub,1);
	howManyWord(&masterLinkedList,&thirdSub,2);
	FILE *out=fopen("output.txt","w");//create file for print orders in output.txt 
	printWordsEachCategory(&firstSub,&secondSub,&thirdSub,&out); //print most frequent 5 words in console
	
	sort(&firstSub);//sort the list with idf value
	
	sort(&secondSub);
	
	sort(&thirdSub);
	
	printWordsEachCategoryIDF(&firstSub,&secondSub,&thirdSub,&out);//print the idf
	
	firstOrderFind(&masterLinkedList);//calculate first Order for every node elements in masterLinkedList
	printf("First Orders were created\n");
	secondOrderFind(&masterLinkedList);//calculate second Order for every node elements in masterLinkedList
	printf("Second Orders were created\n");
	thirdOrderFind(&masterLinkedList);//calculate third Order for every node elements in masterLinkedList
	printf("Third Orders were created\n");
	tempMaster=masterLinkedList;
	
	
	printf("Writing to File.");
	//print the Orders List in the output.txt 
	while(tempMaster!=NULL){
		thirdHead=tempMaster->third;
		secondHead=tempMaster->second;
		firstHead=tempMaster->first;
		fprintf(out,"<-{  %s  }->\n\n",tempMaster->word);
		fprintf(out,"First Order:\n");
		while(firstHead!=NULL){
			fprintf(out,"[%s]\n",firstHead->node->word);
			firstHead=firstHead->next;
		}fprintf(out,"\n");
		fprintf(out,"Second Order:\n");
		while(secondHead!=NULL){
			fprintf(out,"[%s]\n",secondHead->node->word);
			secondHead=secondHead->next;
		}fprintf(out,"\n");
		fprintf(out,"Third Order:\n");
		while(thirdHead!=NULL){
			fprintf(out,"[%s]\n",thirdHead->node->word);
			thirdHead=thirdHead->next;
		}
		fprintf(out,"\n");
		tempMaster=tempMaster->next;
	}
	fclose(out);
	printf("Program finished..\n");
	printf("Big-O(n^4) of my code.\n");
	return 0;
}
