[toc]

# Adversaries ML

Protect the **data**, the **model**, and the **query** (Sensitive).



**Training:** Data Poisoning Attack

- Scenario: Spammers rating emails to affect classifier.
  
  - Real-world problem
- Kinds of Attacks
  
    - Train model to misclassify their spam as spam.
      
    - Train model to misclassify everything (DoS attack). why?
      
    - Train model to classify competitor email as spam.
- Responses
    - Careful validation of training data 
    - Reject on negative impact (RONI) defense
    - Personalization  