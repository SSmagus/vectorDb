# Vector Database Engine (C++)

A lightweight vector similarity search engine implemented from scratch in C++, designed to support efficient k-NN queries on high-dimensional embeddings using clustering-based candidate pruning.

---

## Features

* k-Nearest Neighbor (k-NN) vector search
* L2 / dot-product similarity metrics
* Vector normalization support
* Centroid-based clustering for approximate search
* Configurable number of clusters and probe count (`nprobe`)
* Bucketed inverted index per centroid
* Built-in benchmarking for recall and latency
* Handles 10k+ vectors with dimensions up to 1k

---

## How It Works

### Index Construction

1. Vectors are normalized (optional)
2. A fixed number of centroids are selected
3. Each vector is assigned to its nearest centroid
4. Vectors are stored in centroid-specific buckets

### Query Execution

1. Query vector is normalized
2. Nearest centroids are selected (`nprobe`)
3. Search is restricted to vectors in selected buckets
4. Top-k most similar vectors are returned

This reduces naive **O(N)** search to **O(N / C)** candidate scanning, where `C` is the number of clusters.

---

## Core Components

* **Vector Storage** – stores vector IDs and embeddings
* **Centroid Index** – maintains cluster centroids
* **Bucket Index** – maps centroids to vector IDs
* **Query Engine** – performs approximate nearest neighbor search
* **Benchmark Module** – evaluates recall@k and query latency

---

## Benchmarking

The engine includes benchmarking utilities to evaluate approximation quality.

### Metrics

* Recall@K
* Query latency
* Effect of `nprobe` on recall

Example output:

```
Recall@10 with nprobe=1 : 0.62
Recall@10 with nprobe=3 : 0.81
Recall@10 with nprobe=5 : 0.91
```

Higher `nprobe` improves recall at the cost of increased computation.

---

## Build & Run

```bash
g++ -O2 main.cpp -o vectordb
./vectordb
```

Datasets and parameters can be configured inside the source.

---

## Design Trade-offs

* Approximate search favors speed over exact accuracy
* Clustering quality directly impacts recall
* Increasing `nprobe` improves accuracy but increases query time
* Memory layout optimized for sequential access and cache locality

---

## Future Improvements

* Product Quantization (PQ)
* HNSW graph-based indexing
* SIMD-optimized distance computation
* Multithreaded query execution
* Memory-mapped vector storage

---

## Resume Summary

**Vector Database Engine (C++)**

* Implemented a vector similarity search engine from scratch supporting k-NN queries on high-dimensional embeddings
* Reduced naive O(N) search using centroid-based clustering and candidate pruning
* Benchmarked recall@k and query latency across varying probe counts
* Designed core indexing and search components without external li
