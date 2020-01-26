# Hidden Markov Models: Bio Prediction

Machine learning and Hidden Markov Models application to transmembrane protein secondary structure prediction. Awarded as the best final-year Informatics project in Serbia.

The publication can be found [here](https://www.mg.edu.rs/uploads/files/images/stories/dokumenta/maturski/marina-ivanovic.pdf).

## What are Hidden Markov Models?

Markov Chain is a system that expresses transitions from one state to another with certain probabilities. 

A Hidden Markov Model (HMM) is a Markov Chain in which the state sequence is unobservable. What can be observed is a sequence of outputs. Every produced output at a given moment depends only on the current state.

| <img src="images/chain.png"> | <img src="images/model.png">|
|:---:|:---:|
| Example of a Markov Chain | Example of a Hidden Markov Model |

## How HMMs can be applied to protein structure prediction?

Protein structure can be modeled as HMM, where every amino acid can be modeled as output, while its position as state.

| <img src="images/bio.png"> |
|:---:|
| Example of a protein and its position relative to a cell membrane |

## What algorithms are implemented in this project?

Among other, Baum-Welch algorithm is implemented for the unknown parameters estimation, while Viterbi algorithm is implemented for finding the most likely sequence of hidden states.
