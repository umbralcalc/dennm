# `dennm`: (DEN)sity of (N)on-(M)arkovian phenomena

A generalised density solver for non-Markovian phenomena written in `Rust` and using the `ArrayFire` wrapper ([see here](https://github.com/arrayfire/arrayfire-rust)) to make it nice and fast.

The computational framework for `dennm` is derived using the formalism from the publicly-available book: [Diffusing Ideas](https://umbralcalc.github.io/diffusing-ideas/). Let's begin by writing the generalised non-Markovian master equation for stochastic phenomena which was given in [this chapter](https://umbralcalc.github.io/diffusing-ideas/empirical_probabilistic_reweighting/chapter.pdf) of the book

$$
\begin{align}
P_{{\sf t}+1}(x\vert z) &= \int_{\Omega_{{\sf t}}}{\rm d}X' P_{{\sf t}+1}(X\vert z) = \int_{\Omega_{{\sf t}}}{\rm d}X' P_{{\sf t}}(X'\vert z) P_{({\sf t}+1){\sf t}}(x\vert X',z) .
\end{align}
$$

If now we perform a kind of Kramers-Moyal expansion on this expression like this

$$
\begin{align}
P_{{\sf t}+1}(x\vert z) &= \frac{1}{{\sf t}}\sum_{{\sf t}'=0}^{{\sf t}}\int_{\omega_{{\sf t}'}}{\rm d}^nx' P_{{\sf t}}(X'\vert z) P_{({\sf t}+1){\sf t}}(x\vert X',z) \\
&= \frac{1}{{\sf t}}\sum_{{\sf t}'=0}^{{\sf t}}\int_{\omega_{{\sf t}'}}{\rm d}^nx' P_{{\sf t}}(X_{x'\rightarrow x}\vert z) \bigg\lbrace 1+\sum_{m=1}^{\infty} \frac{\partial^m}{\partial x^m}\big[(x-x')^mP_{({\sf t}+1){\sf t}}(x'\vert X_{x'\rightarrow x},z) \big] \bigg\rbrace ,
\end{align}
$$

which, when truncating the expansion terms up to second order, results in the following difference equation

$$
\begin{align}
P_{{\sf t}+1}(x\vert z) - P_{{\sf t}}(x\vert z) = \frac{1}{{\sf t}}\sum_{{\sf t}'=0}^{{\sf t}}\int_{\omega_{{\sf t}'}}{\rm d}^nx' P_{{\sf t}}(X_{x'\rightarrow x}\vert z) \sum_{m=1}^{\infty}\frac{\partial^m}{\partial x^m}\big[(x-x')^mP_{({\sf t}+1){\sf t}}(x'\vert X_{x'\rightarrow x},z) \big] .
\end{align}
$$
