# `dennm`: (DEN)sity of (N)on-(M)arkovian phenomena

A generalised density solver for non-Markovian phenomena written in `Rust` and using the `ArrayFire` wrapper ([see here](https://github.com/arrayfire/arrayfire-rust)) to make it nice and fast.

The computational framework for `dennm` is derived using the formalism from the publicly-available book: [Diffusing Ideas](https://umbralcalc.github.io/diffusing-ideas/). Let's begin by writing the generalised non-Markovian master equation for stochastic phenomena which was given in [this chapter](https://umbralcalc.github.io/diffusing-ideas/empirical_probabilistic_reweighting/chapter.pdf) of the book

$$
\begin{align}
P_{{\sf t}+1}(x\vert z) &= \int_{\Omega_{{\sf t}}}{\rm d}X' P_{{\sf t}+1}(X\vert z) = \int_{\Omega_{{\sf t}}}{\rm d}X' P_{{\sf t}}(X'\vert z) P_{({\sf t}+1){\sf t}}(x\vert X',z) .
\end{align}
$$

For now, let's imagine that $x$ is just a scalar (as opposed to a row vector) for simplicity in the expressions. We can then write down a kind of Kramers-Moyal expansion of the equation above

$$
\begin{align}
P_{{\sf t}+1}(x\vert z) = \int_{\Omega_{{\sf t}}}{\rm d}X' P_{{\sf t}}(X'\vert z) \bigg[ \delta (x-x') &+ \sum_{{\sf t}'=0}^{{\sf t}}\partial_x\delta (x-x')[\mu_1]_{{\sf t}'}(X'',z) \\ 
&+ \sum_{{\sf t}'=0}^{{\sf t}}\partial_x\delta (x-x')\sum_{{\sf t}''=0}^{{\sf t}'}\partial_{x'}\delta (x'-x'')[\mu_2]_{{\sf t}'{\sf t}''}(X'',z) \dots \bigg] ,
\end{align}
$$

which, when truncating the expansion terms up to second order, results in the following difference equation

$$
\begin{align}
P_{{\sf t}+1}(x\vert z) - P_{{\sf t}}(x\vert z) \simeq &-\sum_{{\sf t}'=0}^{{\sf t}}\frac{\partial}{\partial x}\bigg\lbrace P_{{\sf t}'}(X\vert z)[\mu_1]_{{\sf t}'}(X,z) \bigg\rbrace \bigg\vert_{X_{{\sf t}'}=x} \\
&+\sum_{{\sf t}'=0}^{{\sf t}}\frac{\partial}{\partial x}\sum_{{\sf t}''=0}^{{\sf t}'}\frac{\partial}{\partial x'}\bigg\lbrace P_{{\sf t}'}(X\vert z)[\mu_2]_{{\sf t}'{\sf t}''}(X,z) \bigg\rbrace \bigg\vert_{X_{{\sf t}'}=x \,\wedge\, X_{{\sf t}''}=x'} .
\end{align}
$$
