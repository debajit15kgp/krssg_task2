
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/core/core.hpp"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <stdlib.h>
#include <math.h>
#include<bits/stdc++.h>  
#include<algorithm>
#include<fstream>

using namespace std;
using namespace cv;

#define ROW 600
#define COL 600
#define step 30

typedef struct point
{
    int x,y,di;
}point;


int dist(point p1,point p2)
{
    return (p1.x-p2.x)*(p1.x-p2.x)+(p1.y-p2.y)*(p1.y-p2.y);
}


Mat img=imread("task2.png",1);
int mincost(vector<point>vec)
{
    point p;
    int min=10000000;
    for (int j=0;j<vec.size()-1;j++)
    {
        if (dist(vec[vec.size()-1],vec[j])<600*600)
        {
            if (min>dist(vec[vec.size()-1],vec[j]))
            {
                min=dist(vec[vec.size()-1],vec[j]);
                p=vec[j];
            }
        }
    }
    line(img,Point(p.x,p.y),Point(vec[vec.size()-1].x,vec[vec.size()-1].y),Scalar(255,255,0),1);
    for (int j=0;j<vec.size()-1;j++)
    {
        if (dist(vec[vec.size()-1],vec[j])<1200)
        {
            if (vec[j].di>(p.di+dist(vec[vec.size()-1],vec[j])+min))
            {
                line(img,Point(p.x,p.y),Point(vec[j].x,vec[j].y),Scalar(0,0,0),1);
                line(img,Point(vec[vec.size()-1].x,vec[vec.size()-1].y),Point(p.x,p.y),Scalar(0,255,0),1);
                vec[j].di=p.di+dist(vec[vec.size()-1],vec[j])+min;
            }
        }
    }

    
}

Mat img2(ROW,COL,CV_8UC1,Scalar(0));
int isvalid(point p,point q);
int s;
int main(int argc, char *argv[])
{
    int arr[1000];
    int arrtheta[1000];
    Mat img1=imread("task2.png",0);
    //imshow("win",img1);
    //waitKey(0);
    for(int i=0;i<25;i++)
    {
        for(int j=0;j<25;j++)
        {
            if (sqrt(j*j+i*i)<25)
            {
                img.at<Vec3b>(i,j)[0]=0;
                img.at<Vec3b>(i,j)[1]=255;
                img.at<Vec3b>(i,j)[2]=255;
            }
        }
    }
    for(int i=0;i<ROW;i++)
    {
        for(int j=0;j<COL;j++)
        {
            if (img1.at<uchar>(i,j)>130)
            {
                img2.at<uchar>(i,j)=255;
            }
        }
    }
    //imshow("win",img2);
    //waitKey(0);
    point init={0,0,0};
    //point fin={ROW,COL};
    srand(time(NULL));
    vector<point>vec;
    vector< pair <int,int> >drawx(img.rows*img.rows);
    vector< pair <int,int> >drawy(img.cols*img.cols);
    vec.push_back(init);
    //draw.push_back(0);
    int y=0;
    int l=0;
    while(1)
    {
        int min=10000000;
        int row=rand()%ROW;
        int col=rand()%COL;
        int d=vec[vec.size()-1].di;
        point randp={row,col,d+step};
        point chk;
        for (int i=0;i<vec.size();i++)
        {
            //cout<<dist(randp,vec[i])<<endl;
            if (min>dist(randp,vec[i]))
            {
                min=dist(randp,vec[i]);
                chk=vec[i];
            }
        }

        float t = (float(randp.x - chk.x))/(randp.y - chk.y);
        float theta = abs(atan(t)); 
        //cout<<"1"<<endl;

        if((randp.y>chk.y)&&(randp.x>chk.x))
        {
                randp.y = (int)(chk.y + step*cos(theta));
                randp.x = (int)(chk.x + step*sin(theta));
        }
        else if((randp.y>chk.y)&&(randp.x<chk.x))
        {
                randp.y = (int)(chk.y + step*cos(theta));
                randp.x = (int)(chk.x - step*sin(theta));
        }
        else if((randp.y<chk.y)&&(randp.x>chk.x))
        {
                randp.y = (int)(chk.y - step*cos(theta));
                randp.x = (int)(chk.x + step*sin(theta));
        }
        else
        {
                randp.y = (int)(chk.y - step*cos(theta));
                randp.x = (int)(chk.x - step*sin(theta));
        }
        //cout<<"12"<<endl;
        //cout<<randp.x<<" "<<randp.y<<endl;
        if(isvalid(randp,chk))
        {
                drawx[y].first=randp.x;
                drawx[y].second=chk.x;
                drawy[y].first=randp.y;
                drawy[y].second=chk.y;
                line(img, Point(chk.x,chk.y), Point(randp.x,randp.y), Scalar(0,255,0),1);
                vec.push_back(randp);
                mincost(vec);
                y+=1;
                imshow("win",img);
                waitKey(1);
        }
        //vec.push_back(randp);
        /*if (isCos(tmin(chk))
        {
            line(img1, Point(chk.x,chk.y), Point(randp.x,randp.y), Scalar(255,0,0), 1);
        }*/
        //cout<<"-------------"<<endl;
        ofstream myfile;
        myfile.open("save.txt");
        if (((randp.x-ROW)*(randp.x-ROW)+(randp.y-COL)*(randp.y-COL))<step*step)
        {
            int k=y-1;
            int g=y-1;
            //cout<<k;
            int oldx,oldy,newx,newy;
            int h=0;
            while(1)
            {
                h++;
                newx=drawx[k].first;
                newy=drawy[k].first;
                //cout<<newy<<endl;
                oldx=drawx[k].second;
                oldy=drawy[k].second;

                for (int i=0;i<y;i++)
                {
                    if ((drawx[i].first==drawx[k].second) && (drawy[i].first==drawy[k].second))
                    {
                        s=k;
                        k=i;
                    }
                }
                myfile<<drawx[k].first<<" ";
                myfile<<drawy[k].first<<" "<<endl;

                point p1={newx,newy,0};
                point p2={oldx,oldy,0};
                //cout<<h<<endl;
                //myfile<<sqrt((drawx[k].first-drawx[k].second)*(drawx[k].first-drawx[k].second)+(drawy[k].first-drawy[k].second)*(drawy[k].first-drawy[k].second))<<",";
                //myfile<<atan(float((drawy[k].first-drawy[k].second))/(drawx[k].first-drawx[k].second))-atan(float((drawy[s].first-drawy[s].second))/(drawx[s].first-drawx[s].second))<<",";
                if (isvalid(p1,p2))
                    line(img, Point(newx,newy), Point(oldx,oldy), Scalar(0,0,255),1);
                //cout<<k<<endl;
                if (k==0)
                {
                    break;
                }
            }
            myfile.close();
            cout<<h<<endl;
            while(1)
            {
                newx=drawx[g].first;
                newy=drawy[g].first;
                //cout<<newy<<endl;
                oldx=drawx[g].second;
                oldy=drawy[g].second;

                for (int i=0;i<y;i++)
                {
                    if ((drawx[i].first==drawx[g].second) && (drawy[i].first==drawy[g].second))
                    {
                        s=g;
                        g=i;
                    }
                }
                cout<<sqrt((drawx[s].first-drawx[s].second)*(drawx[s].first-drawx[s].second)+(drawy[s].first-drawy[s].second)*(drawy[s].first-drawy[s].second))<<endl;
                if (g==0)
                {
                    break;
                }
            }

            for (int i=drawx[y-1].first-15;i<drawx[y-1].first+15;i++)
            {
                for(int j=drawy[y-1].first-15;j<drawy[y-1].first+15;j++)
                {
                    if(sqrt((i-drawx[y-1].first)*(i-drawx[y-1].first)+(j-drawy[y-1].first)*(j-drawy[y-1].first))<15)
                    {
                        img.at<Vec3b>(i,j)[0]=0;
                        img.at<Vec3b>(i,j)[1]=255;
                        img.at<Vec3b>(i,j)[2]=255;
                    }
                }
            }
            line(img, Point(newx,newy), Point(oldx,oldy), Scalar(0,0,255),1);
            //cout<<"2"<<endl;
            break;
        }
    }
    //cout<<vec[0].x<<vec[0].y<<endl;
    /*cout<<vec[2].x<<vec[2].y<<endl;
    //cout<<dist(vec[0],vec[8])<<endl;
    //cout<<dist(vec[8],vec[16])<<endl;
    int k1=(int)((y-1)/50);
    //cout<<k1<<endl;
    for (int i=0;i<vec.size();i=i+k1)
    {
        //if ((i+k1)<vec.size())
        cout<<sqrt(dist(vec[i],vec[i+k1]))<<",";
        arrtheta[l]=atan((float(vec[i+k1].y-vec[i].y))/(vec[i+k1].x-vec[i].x));
        //cout<<arr[l]<<",";
        //cout<<arrtheta[l]-arrtheta[l-1]<<",";
        l++;

    Workshops Events Blog Tuto
    }*/

    //cout<<"0----"<<ROW<<COL<<endl;
    //namedWindow("win",0);
    imshow("win",img);
    waitKey(0);
    return 0;
}
int isvalid(point p,point q)
{
    int flag=0;
    float slope=(float(q.x-p.x))/(q.y-p.y);
    slope=atan(slope);
    cout<<1<<endl;
    for (int i=0;i<q.x-p.x+1;i++)
    {
        int t1=q.x+i*sin(slope);
        int t2=q.y+i*cos(slope);
        if ((t1<0 || t1>=ROW) || (t2<0 || t2>COL))
            continue;
        if (img2.at<uchar>(t2,t1)==255)
        {    
            flag=1;
            break;
        }
    }
    //cout<<flag<<endl;
    if((p.x<0 || p.x>=ROW) || (p.y<0 || p.y>COL))
        return 0;
    else if((img2.at<uchar>(p.y,p.x)==255) || (flag==1))
        return 0;
    else
        return 1;
}
        