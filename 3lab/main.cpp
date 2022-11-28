#include <iostream>
#include "geodeb.h"
#include <vector>
#include <cmath>
#include <ctime>


using namespace std;


struct point{
    float x, y;
    bool classified = false;
    int cluster = 0;
};

string COLORS[10] = {"#A52A2A",
                     "#5F9EA0",
                     "#7FFF00",
                     "#FF7F50",
                     "#DC143C",
                     "#006400",
                     "#FF8C00",
                     "#2F4F4F",
                     "#9400D3",
                     "#FF1493"};

int color_id = 0;

void draw(point *arr, int &p_index, const string &color){

    GD_POINT(arr[p_index].x, arr[p_index].y, color);

}


vector<int> getPointQuery(point *arr, const int &N, const int &point_index, const float &eps){
    vector<int> query;

    const point p = arr[point_index];
    const float x = p.x; const float y = p.y;

    for(int i = 0; i < N; i++){
        if (arr[i].classified){
            continue;
        }
        float x0 = arr[i].x, y0 = arr[i].y;
        float distance = sqrt(powf(x - x0, 2) + powf(y - y0, 2));

        if(distance <= eps){
            query.push_back(i);
        }
    }

    return query;
}

void remove(vector<int> &v, const int &elem){
    for (int i = 0; i < v.size(); i++){
        if (v[i] == elem){
            v.erase(v.begin() + i);
        }
    }
}

void changeStatus(point *arr, vector<int> &query, int cluster){
    for (int i : query){
        arr[i].classified = true;
        arr[i].cluster = cluster;
    }
}



bool expandCluster(point *arr, const int &N, const int point_index, int cluster_id, const float &eps, const int minPts){
    vector<int> pointQuery = getPointQuery(arr, N, point_index, eps);

    if(pointQuery.size() < minPts){
        for(int k : pointQuery){
            draw(arr, k, "#000000"); //black color for noises
        }
        changeStatus(arr, pointQuery, 0); //change status for query
        remove(pointQuery, point_index);

        return false;
    }
    else{ //core

        changeStatus(arr, pointQuery, cluster_id);
        for(int k : pointQuery){
            draw(arr, k, COLORS[color_id]);
        }

        remove(pointQuery, point_index);

        while(!pointQuery.empty()){
            int currentPoint = pointQuery.front();
            pointQuery.erase(pointQuery.begin());

            vector<int> result = getPointQuery(arr, N, currentPoint, eps); //calculate neighbours for point from stack

            if(result.size() >= minPts){ //if currentPoint is core
                for(int i : result){
                    if(!arr[i].classified || arr[i].cluster == 0){
                        if(!arr[i].classified && i != currentPoint){
                            pointQuery.push_back(i);
                        }
                        arr[i].cluster = cluster_id; //add to cluster
                        arr[i].classified = true;

                        draw(arr, i, COLORS[color_id]);
                    }
                }
            }

            else if(!result.empty() && result.size() < minPts){
                changeStatus(arr, result, cluster_id);
                for (int i : result){
                    pointQuery.push_back(i);
                    draw(arr, i, COLORS[color_id]);
                }
            }
            remove(pointQuery, currentPoint);
        }
        return true;
    }
}



int main() {
    GD_INIT("graph.html");

    const float eps = 2.0; const int minPts = 2;
    int N = 70;

    point arr[N];
    float X = 17.0 ;
    srand(time(nullptr));
    for(int j = 0; j < N; j++){
        arr[j].x = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/X));
        arr[j].y = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/X));
        cout << "X: " << arr[j].x << ' ' << "Y: " << arr[j].y << endl;
    }

    int cluster_id = 1;
    for(int i = 0; i < N; i++){
        if (!arr[i].classified){
            if(expandCluster(arr, N, i, cluster_id, eps, minPts)){
                cluster_id += 1;
                color_id += 1;

            }
        }
    }
    for(point i : arr){
        cout << i.cluster << endl;
    }








    return 0;
}
