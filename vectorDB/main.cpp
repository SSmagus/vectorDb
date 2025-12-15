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
        cout<<"inserted"<<endl;
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

int main() {
    const int DIM=4;
    VectorDB db(DIM);

    while (true) {
        string cmd;
        cin >> cmd;

        if (cmd == "insert") {
            int id;
            cin >> id;
            vector<float> v(DIM);
            for (int i = 0; i < DIM; i++) cin >> v[i];
            db.insert(id, v);
        }

        else if (cmd == "query") {
            vector<float> v(DIM);
            for (int i = 0; i < DIM; i++) cin >> v[i];
            int k;
            cin >> k;
            auto res = db.query(v, k);
            for (int id : res) cout << id << " ";
            cout << "\n";
        }

        else if (cmd == "exit") {
            break;
        }
    }
}

