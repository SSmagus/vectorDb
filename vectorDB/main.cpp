#include <bits/stdc++.h>
using namespace std;

class VectorDB {
private:
    int dim;

    vector<int> ids;
    vector<vector<float>> vectors;

    vector<float> normalize(const vector<float>& v){
        float tsum=0;

        for(const float& i: v) tsum+=i*i;

        float l2= sqrt(tsum);

        if(l2==0){
            return vector<float> (v.size(), 0.0f);
        }

        vector<float> normalizedVector= v;

        for(float& i: normalizedVector){
            i=i/l2;
        }

        return normalizedVector;
    }
    float dot(const vector<float>& a, const vector<float>& b){
        float res=0;
        for(int i=0; i<dim; i++){
            res+=a[i]*b[i];
        }
        return res;
    }

public:

    VectorDB(int dim){
        this->dim=dim;
    }

    
    void insert(int id, const vector<float>& vec){
        assert((int)vec.size() == dim);

        ids.push_back(id);
        vectors.push_back(normalize(vec));
    }

    
    vector<int> query(const vector<float>& vec, int k){
        assert((int)vec.size() == dim);

        priority_queue<pair<float, int>,vector<pair<float, int>>, greater<pair<float, int>>> heap;
        vector<float> normalizedVec= normalize(vec);

        int n=vectors.size();
        
        for(int i=0; i<n; i++){
            float dotProduct= dot(vectors[i], normalizedVec);
            heap.push({dotProduct, ids[i]});
            if(heap.size()>k) heap.pop();
        }

        int matches=min(k, (int)heap.size());
        vector<int> res(matches);

        for(int i=0; i<matches; i++){
            res[i]=heap.top().second;
            heap.pop();
        }

        reverse(res.begin(), res.end());

        return res;
    }
};
