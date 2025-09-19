## Engine.cpp

```cpp
// BUG: zugzwang positions may still arise, atp, NMP will likely fail
// To potentially avoid this, we could do another full window search
// TODO: Implement null move pruning- conditions to NOT use NMP:
// the side to move is in check [+]
// the side to move has only its king and pawns remaining
// the side to move has a small number of pieces remaining
// the previous move in the search was also a null move.
// DO NOT use nmp in endgames, it'll result in zugzwang cases (having to move results in a loss)
if (this->isNMP(isPVNode, isParentInCheck, depth, ply)) {
    this->makeNullMove();

    int score = -this->search(-beta, -beta + 1, depth - 1 - this->_NMP_REDUCTION, ply + 1);

    this->unmakeNullMove();

    if (score >= beta) {
        return beta;
    }
}
```


