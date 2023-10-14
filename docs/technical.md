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

For now, let's imagine that $x$ is just a scalar (as opposed to a row vector) for simplicity in the expressions. We can then write down a kind of Kramers-Moyal expansion of the equation above

$$
\begin{aligned}
P_{{\sf t}+1}(x\vert z) = \int_{\Omega_{{\sf t}}}{\rm d}X' P_{{\sf t}}(X'\vert z) \bigg[ \delta (x-x') &+ \sum^{{\sf t}}_{{\sf t}'=0}\partial_x\delta (x-x')[\mu_1]_{{\sf t}'}(X'',z) \\
&+ \sum^{{\sf t}}_{{\sf t}'=0}\partial_x\delta (x-x')\sum^{{\sf t}'}_{{\sf t}''=0}\partial_{x}\delta (x-x'')[\mu_2]_{{\sf t}'{\sf t}''}(X'',z) + \dots \bigg] ,
\end{aligned}
$$

where $\partial_x\delta$ in some sense denotes the formal derivative of a Dirac delta function and the $\mu_i$ terms denote the $i$-th conditional moments with respect to the distribution over $x$ at all of the previous timesteps (which are indicated by the timestep indices).

Truncating the expansion terms up to second order results in the following difference equation

$$
\begin{aligned}
P_{{\sf t}+1}(x\vert z) - P_{{\sf t}}(x\vert z) \simeq &-\sum^{{\sf t}}_{{\sf t}'=0}\frac{\partial}{\partial x}\bigg\lbrace \int_{\Omega_{{\sf t}'}}{\rm d}X P_{{\sf t}'}(X\vert z)[\mu_1]_{{\sf t}'}(X,z) \bigg\rbrace_{X_{{\sf t}'}=x} \\
&+\sum^{{\sf t}}_{{\sf t}'=0}\sum^{{\sf t}'}_{{\sf t}''=0}\frac{\partial^2}{\partial x^2}\bigg\lbrace \int_{\Omega_{{\sf t}'}}{\rm d}X P_{{\sf t}'}(X\vert z)[\mu_2]_{{\sf t}'{\sf t}''}(X,z) \bigg\rbrace_{X_{{\sf t}'}=x \,\wedge\, X_{{\sf t}''}=x} .
\end{aligned}
$$

Let's now discretise the derivatives and integral to make the expression more compatible with numerical computation like this

$$
\begin{aligned}
[P]^i_{{\sf t}+1} - [P]^i_{{\sf t}} \simeq &-\sum^{{\sf t}}_{{\sf t}'=0}\bigg\lbrace [P]^{i+1}_{{\sf t}'}[\mu_1]^{(i+1)(i+1)}_{{\sf t}'} - [P]^{i}_{{\sf t}'}[\mu_1]^{ii}_{{\sf t}'} \bigg\rbrace -\sum^{{\sf t}}_{{\sf t}'=0}\sum_{\forall i'\neq i}\bigg\lbrace [P]^{i'}_{{\sf t}'}[\mu_1]^{(i+1)i'}_{{\sf t}'} - [P]^{i'}_{{\sf t}'}[\mu_1]^{ii'}_{{\sf t}'} \bigg\rbrace\\
&+\frac{1}{\Delta x}\sum^{{\sf t}}_{{\sf t}'=0}\sum^{{\sf t}'}_{{\sf t}''=0}\bigg\lbrace [P]^{i+1}_{{\sf t}'}[\mu_2]^{(i+1)(i+1)}_{{\sf t}'} + [P]^{i-1}_{{\sf t}'}[\mu_2]^{(i-1)(i+1)}_{{\sf t}'} - 2[P]^{i}_{{\sf t}'}[\mu_2]^{ii}_{{\sf t}'}  \bigg\rbrace\\
&+\frac{1}{\Delta x}\sum^{{\sf t}}_{{\sf t}'=0}\sum^{{\sf t}'}_{{\sf t}''=0} \sum_{\forall i'\neq i}\bigg\lbrace [P]^{i'}_{{\sf t}'}[\mu_2]^{(i+1)i'}_{{\sf t}'} + [P]^{i'}_{{\sf t}'}[\mu_2]^{(i-1)i'}_{{\sf t}'} - 2[P]^{i'}_{{\sf t}'}[\mu_2]^{ii'}_{{\sf t}'} \bigg\rbrace .
\end{aligned}
$$

For a stable solution of this system of equations $[P]^i_{{\sf t}+1} - [P]^i_{{\sf t}} = {\cal F}^i_{{\sf t}}$, it makes sense to use the [Crank-Nicolson method](https://en.wikipedia.org/wiki/Crank%E2%80%93Nicolson_method) $[P]^i_{{\sf t}+1} - [P]^i_{{\sf t}} = ({\cal F}^i_{{\sf t}+1} + {\cal F}^i_{{\sf t}})/2$.
