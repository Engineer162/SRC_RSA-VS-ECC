**Basis bag nøglegenerering**
- Step 1: P = 896717, Q = 549863.
- Step 2: N = P * Q, N = 493071499771 // (493,071,499,771)
- Step 3: PHI(N) = (P - 1) * (Q - 1), PHI(N) = 493070053192 // (493,070,053,192)
- Step 4: vælg e så at 1 < e < PHI(N) og GCD til (e, PHI(N)) = 1, e = 65537
- Step 5: Udregn den private nøgle d så at d ≡ e^-1