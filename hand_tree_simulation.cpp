#include<iostream>
#include<fstream>
#include<cmath>
#include<algorithm>
#include<queue>
#include<vector>
using namespace std;

struct handjoint
{
    float x, y, z;
    int id;
    int connect[4];
};

struct handpose
{
    string time_stamp;
    handjoint joint[21];
};


//3. Write a function to read row by row a txt file as described above,
//return a pointer that points to an array of struct handPose storing handPoses charged from the file.
float  **read_text(string time)
{
    handpose pose;
    int count=0;
    pose.time_stamp=time;
    ifstream file;

    //open file according to the id of hand pose
    if(time=="0000")
    {
        file.open("hand pose_0000.txt");
    }
    if(time=="0001")
    {
        file.open("hand pose_0001.txt");
    }

    if(count==0)
    {
        file>>pose.time_stamp; 
        cout<<"pose id:"<<pose.time_stamp<<endl;
        count++;    
    }
    //create matrix 21x3 (2d matrix)->write into slots
    float **coord21x3=0;
    coord21x3= new float*[21];
    int i,j;
    for(i=0;i<21;i++)
    {
        coord21x3[i]=new float[3];
        for(j=0;j<3;j++)
        {
            file>>coord21x3[i][j]; //cout<<coord21x3[i][j];
        }
    }
    file.close();
    return coord21x3;
}


//4. a function that calculates the Euclidian distance of every pair of joints of a hand pose. 
//The function returns a pointer that points to 2D array, 
//each element of the array (i,j) is Euclidian distance between ith joint and jth joint.
float **distance_matrix(string time)
{
    handpose pose;
    pose.time_stamp=time;
    float **coord= read_text(pose.time_stamp); //get data
    int i,j; 

    //create matrix 21x21
    float **dis =0;
    dis= new float*[21];
    for(i=0;i<21;i++)
    {
        dis[i]=new float[21];
        for(j=0;j<21;j++)
        {
            //Euclidian distance
            float x=coord[i][0]-coord[j][0];
            float y=coord[i][1]-coord[j][1];
            float z=coord[i][2]-coord[j][2];
            dis[i][j]=sqrt(x*x+y*y+z*z);
            cout<<"from"<<i<<"to"<<j<<":"<<dis[i][j]<<endl;           
        }
    }
    return dis;
}

//5. Write a function that calculates the movement of each joint between two times.
//The function takes two handPoses at timestamp t1 and t2 as input, 
//returns a pointer that points to an 1D array of size 21 elements, 
//each element i of the array is the Euclidian distance of the ith joint at timestamp t1 and ith joint at timestamp t2
float *pose_change(string time1, string time2)
{
    handpose pose1; handpose pose2;
    pose1.time_stamp=time1;
    pose2.time_stamp=time2;
    //get data
    float **coord_1= read_text(pose1.time_stamp);
    float **coord_2= read_text(pose2.time_stamp);
    int i;
    //create array
    float x,y,z;
    float *change=new float[21];
    for(i=0;i<21;i++)
    {
        //Euclidian distance
        x=coord_1[i][0]-coord_2[i][0]; //cout<<x<<endl;
        y=coord_1[i][1]-coord_2[i][1]; //cout<<y<<endl;
        z=coord_1[i][2]-coord_2[i][2]; //cout<<z<<endl;
        change[i]=sqrt(x*x+y*y+z*z); 
        cout<<"joint "<<i<<" move: "<<change[i]<<endl;
    }
    return change;
}

//6.Write a function to sort the array obtained from (5) 
//return the sorted array and the ID of the joint that moves the most.
void sort_max(float arr[21])
{
    //find the joint that moved the most
    float max=arr[0]; int i;
    for(i=0;i<21;i++)
    {
        if(max<arr[i])
        {
            max=arr[i];
        }
    }
    for(i=0;i<21;i++)
    {
        if(max==arr[i])
        {
            cout<<"joint that move the most:"<<i<<endl;
        }
    }

    sort(arr,arr+21); //sort the array
    cout<<"sorted array"<<endl;
    for(i=0;i<21;i++)
    {cout<<arr[i]<<endl;}
}


//7. define a struct Node to represent each joint of hand pose
struct node
{
    int joint_id;
    struct node *left;
    struct node *right;
};


struct node *new_node(int id)
{
    node *temp=new node();
    temp->joint_id=id;
    temp->left=NULL;
    temp->right=NULL;
    return temp;
};

//8.Write a function to insert nodes into the tree, 
//returns the pointer that point the root node 
node* insert(node *root,int id)
{
    node *temp;
    queue <node*> q;
    q.push(root); 
    while(!q.empty())
    {
        //travel through each line of the tree top->bottom, left->right
        temp=q.front();
        q.pop();

        //if left child is empty->insert left
        if(temp->left==NULL)
        {
            temp->left=new_node(id);
            return root;
        }
        else{ q.push(temp->left);}

        //if right child is empty->insert right
        if(temp->right==NULL)
        {
            temp->right=new_node(id);
            return root;
        }
        else
        {
            q.push(temp->right);
        }
    }
}

//9. Write a function to traversal the tree
void pre_order(node *root) //root->left->right
{
    if(root==NULL)
    {
        return;
    }
    else{
        //traversal using recussion
        cout<<root->joint_id<<endl;
        pre_order(root->left);
        pre_order(root->right);
    }
}


void in_order(node *root) //left->root->right
{
    if(root==NULL)
    {
        return;
    }
    else
    {
        in_order(root->left);
        cout<<root->joint_id<<endl;
        in_order(root->right);
    }
}


void post_order(node *root) //left->right->root
{
    if(root==NULL)
    {
        return;
    }
    else{
        post_order(root->left);
        post_order(root->right);
        cout<<root->joint_id<<endl;
    }
}

//10. Write a function that determines longest path of the tree. 
//The function returns to the array storing the 
//ID of node belonging to the longest path. 
vector<int> longest_path(node *root)
{
    if(root==NULL)
    {
        vector<int> temp;
        return temp;
    }
    else
    {
        //calculate the longest path from right and left subtree
        vector<int> vec_right=longest_path(root->right);
        vector<int> vec_left=longest_path(root->left);
        if(vec_left.size()>vec_right.size())
        {
            vec_left.push_back(root->joint_id);
        }
        else
        {
            vec_right.push_back(root->joint_id); 
        }
        return(vec_left.size()>vec_right.size()?vec_left:vec_right); //return the vector with bigger size
    }
}


void choice_display()
{
    cout<<"1.calculate euclidian distance between of every pair of joint"<<endl;
    cout<<"2.calculate movement of joint between 2 time"<<endl;
    cout<<"3.sort the array movement and display the joint that move the most"<<endl;
    cout<<"4.represent then hand joint in binary tree (from 0 to 7 bc i'm lazy)"<<endl;
    cout<<"5.insert a node (123)"<<endl;
    cout<<"6.display the longest path"<<endl;
}


int main()
{
    handpose pose_0000;
    pose_0000.time_stamp="0000";
    handpose pose_0001;
    pose_0001.time_stamp="0001";

    //binary tree implementation
    struct node* root=new_node(0);
    root->left = new_node(1);
    root->right = new_node(2);
    root->left->left = new_node(3);
    root->left->right = new_node(4);
    root->right->left = new_node(5);
    root->left->left->right = new_node(7);

    int choice; int i;
    choice_display();
    cout<<"enter your choice:";
    cin>>choice;
    switch(choice)
    {
        case 1: //euclidian distance of every pair of joints of a hand pose.
            {float **euc_dis_0000=distance_matrix(pose_0000.time_stamp);
            //float **euc_dis_0001=distance_matrix(pose_0001.time_stamp);
            break;}
        case 2: //calculate movemont of each joint
           {float *change_point= pose_change(pose_0000.time_stamp,pose_0001.time_stamp); break;}
        case 3:
            {float *change_point= pose_change(pose_0000.time_stamp,pose_0001.time_stamp);
            sort_max(change_point); break;}
        case 4:
            //binary tree start here
            cout<<"pre-order representation:"<<endl;
            pre_order(root);
            cout<<"in-order representation:"<<endl;
            in_order(root);
            cout<<"post-order representation:"<<endl;
            post_order(root); break;
        case 5: //insert a node
            root=insert(root, 123);
            cout<<"pre-order representation:"<<endl;
            pre_order(root);
            cout<<"in-order representation:"<<endl;
            in_order(root);
            cout<<"post-order representation:"<<endl;
            post_order(root); break;
        case 6: //display longest path1
            {vector <int> path=longest_path(root);
            int n=path.size();//cout<<path[n-1];
            for(i=n-1;i>=0;i--)
            {cout<<path[i]<<"->";} break;}
    }
}




