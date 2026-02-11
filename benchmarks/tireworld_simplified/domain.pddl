(define (domain triangle-tire)
  (:requirements :typing :strips :non-deterministic :conditional-effects :universal-preconditions)
  (:types location)

  (:predicates
    (at ?loc - location)
    (spare-in ?loc - location)
    (connected ?from - location ?to - location)
    (can-move-to ?to - location)
    (not-flattire)
  )

  ;; move(?to) -- no ?from parameter.
  ;; Precondition uses can-move-to so the domain controls possible moves.
  ;; Effects:
  ;;  - set (at ?to) and remove any previous (at ?x)
  ;;  - refresh can-move-to based on outgoing edges from ?to
  ;;  - nondet flat tire
  (:action move
    :parameters (?to - location)
    :precondition (and (not-flattire) (can-move-to ?to))
    :effect (and
      ;; clear old position (unique position encoded by deleting all at(?x))
      (forall (?x - location)
        (when (at ?x) (not (at ?x)))
      )
      (at ?to)

      ;; clear old move options
      (forall (?y - location)
        (when (can-move-to ?y) (not (can-move-to ?y)))
      )

      ;; add new move options: successors of ?to
      (forall (?y - location)
        (when (connected ?to ?y) (can-move-to ?y))
      )

      ;; nondeterministic flat tire after moving
      (oneof
        (and)
        (not (not-flattire))
      )
    )
  )

  (:action changetire
    :parameters (?loc - location)
    :precondition (and (spare-in ?loc) (at ?loc))
    :effect (and (not (spare-in ?loc)) (not-flattire))
  )
)
