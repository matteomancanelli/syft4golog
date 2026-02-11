(define (domain triangle-tire-n21)
  (:requirements :typing :strips :non-deterministic)
  (:types location)
  (:constants l1 l2 l3 l4 l5 l6 l7 l8 l9 l10 l11 l12 l13 l14 l15 l16 l17 l18 l19 l20 l21 - location)
  (:predicates
    (at ?loc - location)
    (spare-in ?loc - location)
    (not-flattire)
  )
  (:action move_l1_l2
    :precondition (and (at l1) (not-flattire))
    :effect (and
      (at l2)
      (not (at l1))
      (oneof (and) (not (not-flattire)))
    )
  )
  (:action move_l2_l1
    :precondition (and (at l2) (not-flattire))
    :effect (and
      (at l1)
      (not (at l2))
      (oneof (and) (not (not-flattire)))
    )
  )
  (:action move_l2_l3
    :precondition (and (at l2) (not-flattire))
    :effect (and
      (at l3)
      (not (at l2))
      (oneof (and) (not (not-flattire)))
    )
  )
  (:action move_l3_l2
    :precondition (and (at l3) (not-flattire))
    :effect (and
      (at l2)
      (not (at l3))
      (oneof (and) (not (not-flattire)))
    )
  )
  (:action move_l3_l4
    :precondition (and (at l3) (not-flattire))
    :effect (and
      (at l4)
      (not (at l3))
      (oneof (and) (not (not-flattire)))
    )
  )
  (:action move_l4_l3
    :precondition (and (at l4) (not-flattire))
    :effect (and
      (at l3)
      (not (at l4))
      (oneof (and) (not (not-flattire)))
    )
  )
  (:action move_l4_l5
    :precondition (and (at l4) (not-flattire))
    :effect (and
      (at l5)
      (not (at l4))
      (oneof (and) (not (not-flattire)))
    )
  )
  (:action move_l5_l4
    :precondition (and (at l5) (not-flattire))
    :effect (and
      (at l4)
      (not (at l5))
      (oneof (and) (not (not-flattire)))
    )
  )
  (:action move_l5_l6
    :precondition (and (at l5) (not-flattire))
    :effect (and
      (at l6)
      (not (at l5))
      (oneof (and) (not (not-flattire)))
    )
  )
  (:action move_l6_l5
    :precondition (and (at l6) (not-flattire))
    :effect (and
      (at l5)
      (not (at l6))
      (oneof (and) (not (not-flattire)))
    )
  )
  (:action move_l6_l7
    :precondition (and (at l6) (not-flattire))
    :effect (and
      (at l7)
      (not (at l6))
      (oneof (and) (not (not-flattire)))
    )
  )
  (:action move_l7_l6
    :precondition (and (at l7) (not-flattire))
    :effect (and
      (at l6)
      (not (at l7))
      (oneof (and) (not (not-flattire)))
    )
  )
  (:action move_l7_l8
    :precondition (and (at l7) (not-flattire))
    :effect (and
      (at l8)
      (not (at l7))
      (oneof (and) (not (not-flattire)))
    )
  )
  (:action move_l8_l7
    :precondition (and (at l8) (not-flattire))
    :effect (and
      (at l7)
      (not (at l8))
      (oneof (and) (not (not-flattire)))
    )
  )
  (:action move_l8_l9
    :precondition (and (at l8) (not-flattire))
    :effect (and
      (at l9)
      (not (at l8))
      (oneof (and) (not (not-flattire)))
    )
  )
  (:action move_l9_l8
    :precondition (and (at l9) (not-flattire))
    :effect (and
      (at l8)
      (not (at l9))
      (oneof (and) (not (not-flattire)))
    )
  )
  (:action move_l9_l10
    :precondition (and (at l9) (not-flattire))
    :effect (and
      (at l10)
      (not (at l9))
      (oneof (and) (not (not-flattire)))
    )
  )
  (:action move_l10_l9
    :precondition (and (at l10) (not-flattire))
    :effect (and
      (at l9)
      (not (at l10))
      (oneof (and) (not (not-flattire)))
    )
  )
  (:action move_l10_l11
    :precondition (and (at l10) (not-flattire))
    :effect (and
      (at l11)
      (not (at l10))
      (oneof (and) (not (not-flattire)))
    )
  )
  (:action move_l11_l10
    :precondition (and (at l11) (not-flattire))
    :effect (and
      (at l10)
      (not (at l11))
      (oneof (and) (not (not-flattire)))
    )
  )
  (:action move_l11_l12
    :precondition (and (at l11) (not-flattire))
    :effect (and
      (at l12)
      (not (at l11))
      (oneof (and) (not (not-flattire)))
    )
  )
  (:action move_l12_l11
    :precondition (and (at l12) (not-flattire))
    :effect (and
      (at l11)
      (not (at l12))
      (oneof (and) (not (not-flattire)))
    )
  )
  (:action move_l12_l13
    :precondition (and (at l12) (not-flattire))
    :effect (and
      (at l13)
      (not (at l12))
      (oneof (and) (not (not-flattire)))
    )
  )
  (:action move_l13_l12
    :precondition (and (at l13) (not-flattire))
    :effect (and
      (at l12)
      (not (at l13))
      (oneof (and) (not (not-flattire)))
    )
  )
  (:action move_l13_l14
    :precondition (and (at l13) (not-flattire))
    :effect (and
      (at l14)
      (not (at l13))
      (oneof (and) (not (not-flattire)))
    )
  )
  (:action move_l14_l13
    :precondition (and (at l14) (not-flattire))
    :effect (and
      (at l13)
      (not (at l14))
      (oneof (and) (not (not-flattire)))
    )
  )
  (:action move_l14_l15
    :precondition (and (at l14) (not-flattire))
    :effect (and
      (at l15)
      (not (at l14))
      (oneof (and) (not (not-flattire)))
    )
  )
  (:action move_l15_l14
    :precondition (and (at l15) (not-flattire))
    :effect (and
      (at l14)
      (not (at l15))
      (oneof (and) (not (not-flattire)))
    )
  )
  (:action move_l15_l16
    :precondition (and (at l15) (not-flattire))
    :effect (and
      (at l16)
      (not (at l15))
      (oneof (and) (not (not-flattire)))
    )
  )
  (:action move_l16_l15
    :precondition (and (at l16) (not-flattire))
    :effect (and
      (at l15)
      (not (at l16))
      (oneof (and) (not (not-flattire)))
    )
  )
  (:action move_l16_l17
    :precondition (and (at l16) (not-flattire))
    :effect (and
      (at l17)
      (not (at l16))
      (oneof (and) (not (not-flattire)))
    )
  )
  (:action move_l17_l16
    :precondition (and (at l17) (not-flattire))
    :effect (and
      (at l16)
      (not (at l17))
      (oneof (and) (not (not-flattire)))
    )
  )
  (:action move_l17_l18
    :precondition (and (at l17) (not-flattire))
    :effect (and
      (at l18)
      (not (at l17))
      (oneof (and) (not (not-flattire)))
    )
  )
  (:action move_l18_l17
    :precondition (and (at l18) (not-flattire))
    :effect (and
      (at l17)
      (not (at l18))
      (oneof (and) (not (not-flattire)))
    )
  )
  (:action move_l18_l19
    :precondition (and (at l18) (not-flattire))
    :effect (and
      (at l19)
      (not (at l18))
      (oneof (and) (not (not-flattire)))
    )
  )
  (:action move_l19_l18
    :precondition (and (at l19) (not-flattire))
    :effect (and
      (at l18)
      (not (at l19))
      (oneof (and) (not (not-flattire)))
    )
  )
  (:action move_l19_l20
    :precondition (and (at l19) (not-flattire))
    :effect (and
      (at l20)
      (not (at l19))
      (oneof (and) (not (not-flattire)))
    )
  )
  (:action move_l20_l19
    :precondition (and (at l20) (not-flattire))
    :effect (and
      (at l19)
      (not (at l20))
      (oneof (and) (not (not-flattire)))
    )
  )
  (:action move_l20_l21
    :precondition (and (at l20) (not-flattire))
    :effect (and
      (at l21)
      (not (at l20))
      (oneof (and) (not (not-flattire)))
    )
  )
  (:action move_l21_l20
    :precondition (and (at l21) (not-flattire))
    :effect (and
      (at l20)
      (not (at l21))
      (oneof (and) (not (not-flattire)))
    )
  )
  (:action changetire
    :parameters (?loc - location)
    :precondition (and (spare-in ?loc) (at ?loc))
    :effect (and (not (spare-in ?loc)) (not-flattire))
  )
)
