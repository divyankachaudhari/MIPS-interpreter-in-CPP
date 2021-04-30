#include <iostream>
#include <cstdlib>
#include <algorithm>
using namespace std;

int main () {
int randomNumberOfPoints;
srand(time(0)); randomNumberOfPoints = rand()%100;
cout<< "The number of points we're going to have are (only between 0 to 100):"<<randomNumberOfPoints<<endl;
//Assuming max value of RAND_max to be 2^32
srand(time(0)+1);
cout<<"Generating x co-ordinates"<<endl;
int xPoints[randomNumberOfPoints];
for(int i=0; i<randomNumberOfPoints;i++){ xPoints[i] = rand() - 65536;}
sort(xPoints, xPoints+randomNumberOfPoints);

srand(time(0)+2);
cout<<"Generating y co-ordinates"<< endl;
int yPoints[randomNumberOfPoints];
for(int j=0; j<randomNumberOfPoints; j++) {
  yPoints[j]= rand()-65536;
}
cout<<"The co-ordinates in the right order are:"<<endl;
for(int k=0; k<randomNumberOfPoints; k++){ cout<<xPoints[k]<<endl; cout<<yPoints[k]<<endl;}

cout<<"Calculating area..."<<endl; double area = 0.0;

for(int i=0; i<randomNumberOfPoints-1; i++){
  if(yPoints[i]==0 | yPoints[i+1]== 0 | (yPoints[i]/abs(yPoints[i])*(yPoints[i+1]/abs(yPoints[i+1]))) >= 0) {
    area = area + 0.5*(xPoints[i+1] - xPoints[i])*(abs(yPoints[i]) + abs(yPoints[i+1]));
  }
  else {
    area = area + 0.5*(xPoints[i+1] - xPoints[i])*(((yPoints[i]*yPoints[i]) + (yPoints[i+1]*yPoints[i+1]))/(abs(yPoints[i])+abs(yPoints[i+1])));
  }
}
cout<<"Expected area output:" << area;
}
