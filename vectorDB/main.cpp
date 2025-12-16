#include <bits/stdc++.h>
using namespace std;

class VectorDB {
private:
    int dim;

    vector<int> ids;
    vector<vector<float>> vectors;
    
    int numClusters;
    vector<vector<float>> centroids;
    vector<vector<int>> buckets; 
    int defaultNprobe;
    int lastIndexedSize;

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

    int closestCentroid(const vector<float>& v){
        float bestMatch=-1e9;
        int bestMatchIndex=-1;

        for(int i=0; i<numClusters; i++){
            float sim= dot(centroids[i], v);
            if(sim>bestMatch){
                bestMatch=sim;
                bestMatchIndex=i;
            }
        }
        return bestMatchIndex;
    }

    vector<int> selectTopCentroids(const vector<float>& query, int nprobe){
        vector<float> normalizedVector= query;
        priority_queue<pair<float, int>, vector<pair<float, int>>, greater<pair<float, int>>> heap;
        int k=min(nprobe, numClusters);

        vector<int> res(k);

        for(int i=0; i<numClusters; i++){
            float sim= dot(normalizedVector, centroids[i]);
            heap.push({sim, i});
            if(heap.size()>k) heap.pop();
        }

        for(int i=0; i<k; i++){
            res[i]=heap.top().second;
            heap.pop();
        }
        return res;
    }


public:

    VectorDB(int dim){
        this->dim=dim;
        lastIndexedSize=0;
        defaultNprobe=1;
    }

    
    void insert(int id, const vector<float>& vec){
        assert((int)vec.size() == dim);

        ids.push_back(id);
        vectors.push_back(normalize(vec));
    }

    
    vector<int> query(const vector<float>& vec, int k, int nprobe=-1){
        assert((int)vec.size() == dim);

        int n=vectors.size();

        if (n == 0 || k <= 0) { return vector<int>(); }

        if(lastIndexedSize == 0 || n>=2*lastIndexedSize){
            int clusters = max(1, (int)sqrt(n));
            buildIndex(clusters);
            lastIndexedSize = n;
        }

        priority_queue<pair<float, int>,vector<pair<float, int>>, greater<pair<float, int>>> heap;
        vector<float> normalizedVec= normalize(vec);
        int probe=nprobe;
    
        if(nprobe==-1){
            probe=defaultNprobe;
        }

        vector<int> c;
        if(probe == 1){
            c={closestCentroid(normalizedVec)};
        }
        else{
            c=selectTopCentroids(normalizedVec, probe);
        }
        
        for(int i: c){
            for(int idx: buckets[i]){
                float dotProduct= dot(vectors[idx], normalizedVec);
                heap.push({dotProduct, ids[idx]});
                if(heap.size()>k) heap.pop();
            }
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

    void buildIndex(int k) {
        int n = vectors.size();

        numClusters = min(k, n);

        vector<int> idx(n);
        iota(idx.begin(), idx.end(), 0);

        random_device rd;
        mt19937 rng(rd());
        shuffle(idx.begin(), idx.end(), rng);

        centroids.clear();
        buckets.assign(numClusters, {});   

        for (int i = 0; i < numClusters; i++) {  
            centroids.push_back(vectors[idx[i]]);
        }

        for (int i = 0; i < n; i++) {
            int c = closestCentroid(vectors[i]);
            buckets[c].push_back(i);
        }
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

