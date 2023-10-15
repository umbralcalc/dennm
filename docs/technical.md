---
geometry: margin=2cm
---

# `dennm-torch`: (DEN)sity of (N)on-(M)arkovian phenomena with Libtorch

The computational framework for `dennm-torch` is derived using the formalism from the publicly-available book: [Diffusing Ideas](https://umbralcalc.github.io/diffusing-ideas/). Let's begin by writing the generalised non-Markovian master equation for stochastic phenomena which was given in [this chapter](https://umbralcalc.github.io/diffusing-ideas/empirical_probabilistic_reweighting/chapter.pdf) of the book

$$
\begin{aligned}
P_{{\sf t}+1}(x\vert z) &= \int_{\Omega_{{\sf t}}}{\rm d}X' P_{{\sf t}+1}(X\vert z) = \int_{\Omega_{{\sf t}}}{\rm d}X' P_{{\sf t}}(X'\vert z) P_{({\sf t}+1){\sf t}}(x\vert X',z) .
\end{aligned}
$$

For now, let's imagine that $x$ is just a scalar (as opposed to a row vector) for simplicity in the expressions. We can then write down a kind of [Kramers-Moyal expansion](https://en.wikipedia.org/wiki/Kramers%E2%80%93Moyal_expansion) of the equation above

$$
\begin{aligned}
P_{{\sf t}+1}(x\vert z) = \int_{\Omega_{{\sf t}}}{\rm d}X' P_{{\sf t}}(X'\vert z) \bigg[ &\frac{1}{{\sf t}}\sum^{{\sf t}}_{{\sf t}'=0}\delta (x-x') - \frac{1}{{\sf t}}\sum^{{\sf t}}_{{\sf t}'=0}\partial_x\delta (x-x')\mu_1 (X',z) \\
&+ \frac{1}{2{\sf t}}\sum^{{\sf t}}_{{\sf t}'=0}\partial_x\delta (x-x')\frac{1}{{\sf t}'}\sum^{{\sf t}'}_{{\sf t}''=0}\partial_{x}\delta (x-x'')\mu_2 (X',z) + \dots \bigg] ,
\end{aligned}
$$

where $\partial_x\delta$ denotes the formal derivative of a Dirac delta function and the $\mu_i$ terms denote the $i$-th conditional moments with respect to the distribution over $x$. Truncating the expansion up to second order and integrating each term by parts results in the following difference equation

$$
\begin{aligned}
P_{{\sf t}+1}(x\vert z) - P_{{\sf t}}(x\vert z) \simeq &-\frac{1}{{\sf t}}\sum^{{\sf t}}_{{\sf t}'=0}\frac{\partial}{\partial x}\bigg\lbrace \int_{\Omega_{{\sf t}'}}{\rm d}X P_{{\sf t}'}(X\vert z)[\mu_1]_{{\sf t}'}(X,z) \bigg\rbrace_{X_{{\sf t}'}=x} \\
&+\frac{1}{2{\sf t}}\sum^{{\sf t}}_{{\sf t}'=0}\frac{1}{{\sf t}'}\sum^{{\sf t}'}_{{\sf t}''=0}\frac{\partial^2}{\partial x^2}\bigg\lbrace \int_{\Omega_{{\sf t}'}}{\rm d}X P_{{\sf t}'}(X\vert z)[\mu_2]_{{\sf t}'{\sf t}''}(X,z) \bigg\rbrace_{X_{{\sf t}'}=x \,\wedge\, X_{{\sf t}''}=x} .
\end{aligned}
$$

Let's now discretise the derivatives and integral to make the expression more compatible with numerical computation like this $[P]^i_{{\sf t}+1} - [P]^i_{{\sf t}} = {\cal J}^i_{{\sf t}}$, where the probability current ${\cal J}^i_{{\sf t}}$ is defined as

$$
\begin{aligned}
{\cal J}^i_{{\sf t}} = &-\frac{1}{{\sf t}}\sum^{{\sf t}}_{{\sf t}'=0}\bigg\lbrace [P]^{i+1}_{{\sf t}'}[\mu_1]^{(i+1)(i+1)}_{{\sf t}'} - [P]^{i}_{{\sf t}'}[\mu_1]^{ii}_{{\sf t}'} \bigg\rbrace - \frac{1}{{\sf t}}\sum^{{\sf t}}_{{\sf t}'=0}\sum_{\forall i'\neq i}\bigg\lbrace [P]^{i'}_{{\sf t}'}[\mu_1]^{(i+1)i'}_{{\sf t}'} - [P]^{i'}_{{\sf t}'}[\mu_1]^{ii'}_{{\sf t}'} \bigg\rbrace\\
&+\frac{1}{2{\sf t}\Delta x}\sum^{{\sf t}}_{{\sf t}'=0}\frac{1}{{\sf t}'}\sum^{{\sf t}'}_{{\sf t}''=0}\bigg\lbrace [P]^{i+1}_{{\sf t}'}[\mu_2]^{(i+1)(i+1)}_{{\sf t}'{\sf t}''} + [P]^{i-1}_{{\sf t}'}[\mu_2]^{(i-1)(i+1)}_{{\sf t}'{\sf t}''} - 2[P]^{i}_{{\sf t}'}[\mu_2]^{ii}_{{\sf t}'{\sf t}''}  \bigg\rbrace\\
&+\frac{1}{2{\sf t}\Delta x}\sum^{{\sf t}}_{{\sf t}'=0}\frac{1}{{\sf t}'}\sum^{{\sf t}'}_{{\sf t}''=0} \sum_{\forall i'\neq i}\bigg\lbrace [P]^{i'}_{{\sf t}'}[\mu_2]^{(i+1)i'}_{{\sf t}'{\sf t}''} + [P]^{i'}_{{\sf t}'}[\mu_2]^{(i-1)i'}_{{\sf t}'{\sf t}''} - 2[P]^{i'}_{{\sf t}'}[\mu_2]^{ii'}_{{\sf t}'{\sf t}''} \bigg\rbrace .
\end{aligned}
$$

For a stable solution of this system of equations, it makes sense to use the [Crank-Nicolson method](https://en.wikipedia.org/wiki/Crank%E2%80%93Nicolson_method) $[P]^i_{{\sf t}+1} - [P]^i_{{\sf t}} = ({\cal J}^i_{{\sf t}+1} + {\cal J}^i_{{\sf t}})/2$.
