Copyright @lucabotez

# DFA Classifier

## Overview
This project constructs a **Deterministic Finite Automaton** that classifies words into **Accepted** and **Rejected** categories based on given language constraints. The DFA is optimized to have the **fewest possible states** while maintaining correct classification.

## Features
- **Trie-based DFA Construction** for efficient state minimization.
- **State Optimization** by merging equivalent states.
- **Fast Classification** using a compact transition matrix.
- **Handles both Accept and Reject word sets**.

## Implementation Details
### **1. Trie Construction**
- A **Trie** is built to store all words from both the **Accept** and **Fail** sets.
- Each node contains **two flags**:
  - **Accept flag**: Indicates if any word in the subtree belongs to the Accept set.
  - **Reject flag**: Indicates if any word in the subtree belongs to the Fail set.
- This structure allows efficient **state determination** when constructing the DFA.

### **2. DFA Creation**
- The DFA starts with **three states**:
  - **Accept state**
  - **Reject state**
  - **Root state** (initial state)
- The Trie is traversed, and for each node:
  - If **both flags are set**, a **new DFA state** is created to extend the prefix for accurate classification.
  - Otherwise, a transition is added **directly to the Accept or Reject state** based on the flags.
  - If only one flag is set, further exploration is unnecessary, as all remaining words in the subtree belong to the same category.

### **3. DFA Optimization**
- To **reduce the number of states**, equivalent states are merged.
- **Equivalent states** are those with transitions leading to the same Accept/Reject states.
- The algorithm iterates through the transition matrix, identifying **identical state pairs** and merging them.
- This process repeats until no further optimizations are possible.

## Notes
- The **Trie structure ensures minimal DFA state creation**.
- **Optimization reduces state redundancy**, improving classification speed.
- The DFA can be further extended for **regex-based classification**.
