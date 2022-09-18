From Coq Require Export String.
(* Bool Algerba *)

Inductive bool : Type :=
    | true
    | false.

Definition negb (b:bool) : bool :=
    match b with
        | true => false
        | false => true
    end.

Definition andb (b1:bool) (b2:bool) : bool :=
    match b1 with
        | true => b2
        | false => false
    end.

Definition orb (b1:bool) (b2:bool) : bool :=
    match b1 with
        | true => true
        | false => b2
    end.

Definition nandb (b1:bool) (b2:bool) : bool :=
  match b1, b2 with
  | true, true => false
  | _, _ => true
  end.

Notation "x && y" := (andb x y).
Notation "x || y" := (orb x y).

(* Natural Algebra*)
Inductive nat : Type :=
        | O (*English alphabet O*)
        | S (n : nat).

Fixpoint eqb (n m : nat) : bool :=
    match n with
        | O => match m with
                | O => true
                | S m' => false
                end
        | S n' => match m with
                    | O => false
                    | S m' => eqb n' m'
                end
    end.

Fixpoint leb (n m : nat) : bool :=
  match n with
  | O => true
  | S n' =>
      match m with
      | O => false
      | S m' => leb n' m'
      end
  end.

Definition ltb (n m : nat) : bool := 
    leb n m.

Definition pred (n : nat) : nat :=
    match n with
        | O => O
        | S n' => n'
    end.

Fixpoint evenb (n:nat) : bool :=
    match n with
        | O => true
        | S O => false
        | S (S n') => evenb n'
    end.

Definition oddb (n:nat) : bool :=
  negb (evenb n).

Fixpoint plus (n : nat) (m : nat) : nat :=
    match n with
        | O => m
        | S n' => S (plus n' m)
    end.

Fixpoint minus (n m:nat) : nat :=
    match n, m with
        | O , _ => O
        | S _ , O => n
        | S n', S m' => minus n' m'
    end.

Fixpoint mult (n m : nat) : nat :=
    match n with
        | O => O
        | S n' => plus m (mult n' m)
    end.

Fixpoint exp (base power : nat) : nat :=
    match power with
        | O => S O
        | S p => mult base (exp base p)
    end.

Fixpoint factorial (n : nat) : nat :=
    match n with
        | O => (S O)
        | S n' => mult (S n') (factorial n')
    end.

Example test_factorial1 : (factorial 3) = 6.
Proof. reflexivity. Qed.

Notation "x + y" := (plus x y) (at level 50, left associativity) : nat_scope.
Notation "x - y" := (minus x y) (at level 50, left associativity) : nat_scope.
Notation "x * y" := (mult x y) (at level 40, left associativity) : nat_scope.
Notation "x =? y" := (eqb x y) (at level 70) : nat_scope.
Notation "x <=? y" := (leb x y) (at level 70) : nat_scope.

Compute (plus (S (S O)) O).
Compute ((S O) + ((S (S O)) * (S (S O)))) :nat.
Compute ((S (S O)) =? (S (S O))).
(* Compute (factorial (S (S (S (S O))))). *)
Compute (mult (S (S (S O))) (S (S (S O)))).