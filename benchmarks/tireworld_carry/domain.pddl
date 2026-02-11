(define (domain tire-carry)
  (:requirements :strips :non-deterministic)
  (:predicates
    (bit0)
    (bit1)
    (bit2)
    (bit3)
    (bit4)
    (bit5)
    (not-flattire)
    (has-spare)
  )

  (:action pickup_spare
    :parameters ()
    :precondition (not (has-spare))
    :effect (has-spare)
  )

  (:action changetire
    :parameters ()
    :precondition (and (has-spare))
    :effect (and (not-flattire) (not (has-spare)))
  )

  (:action move_fwd_p0
    :parameters ()
    :precondition (and (not (bit0)) (not (bit1)) (not (bit2)) (not (bit3)) (not (bit4)) (not (bit5)) (not-flattire))
    :effect (and
      (bit0) (not (bit1)) (not (bit2)) (not (bit3)) (not (bit4)) (not (bit5))
      (oneof (and) (not (not-flattire)))
    )
  )

  (:action move_fwd_p1
    :parameters ()
    :precondition (and (bit0) (not (bit1)) (not (bit2)) (not (bit3)) (not (bit4)) (not (bit5)) (not-flattire))
    :effect (and
      (not (bit0)) (bit1) (not (bit2)) (not (bit3)) (not (bit4)) (not (bit5))
      (oneof (and) (not (not-flattire)))
    )
  )

  (:action move_bwd_p1
    :parameters ()
    :precondition (and (bit0) (not (bit1)) (not (bit2)) (not (bit3)) (not (bit4)) (not (bit5)) (not-flattire))
    :effect (and
      (not (bit0)) (not (bit1)) (not (bit2)) (not (bit3)) (not (bit4)) (not (bit5))
      (oneof (and) (not (not-flattire)))
    )
  )

  (:action move_fwd_p2
    :parameters ()
    :precondition (and (not (bit0)) (bit1) (not (bit2)) (not (bit3)) (not (bit4)) (not (bit5)) (not-flattire))
    :effect (and
      (bit0) (bit1) (not (bit2)) (not (bit3)) (not (bit4)) (not (bit5))
      (oneof (and) (not (not-flattire)))
    )
  )

  (:action move_bwd_p2
    :parameters ()
    :precondition (and (not (bit0)) (bit1) (not (bit2)) (not (bit3)) (not (bit4)) (not (bit5)) (not-flattire))
    :effect (and
      (bit0) (not (bit1)) (not (bit2)) (not (bit3)) (not (bit4)) (not (bit5))
      (oneof (and) (not (not-flattire)))
    )
  )

  (:action move_fwd_p3
    :parameters ()
    :precondition (and (bit0) (bit1) (not (bit2)) (not (bit3)) (not (bit4)) (not (bit5)) (not-flattire))
    :effect (and
      (not (bit0)) (not (bit1)) (bit2) (not (bit3)) (not (bit4)) (not (bit5))
      (oneof (and) (not (not-flattire)))
    )
  )

  (:action move_bwd_p3
    :parameters ()
    :precondition (and (bit0) (bit1) (not (bit2)) (not (bit3)) (not (bit4)) (not (bit5)) (not-flattire))
    :effect (and
      (not (bit0)) (bit1) (not (bit2)) (not (bit3)) (not (bit4)) (not (bit5))
      (oneof (and) (not (not-flattire)))
    )
  )

  (:action move_fwd_p4
    :parameters ()
    :precondition (and (not (bit0)) (not (bit1)) (bit2) (not (bit3)) (not (bit4)) (not (bit5)) (not-flattire))
    :effect (and
      (bit0) (not (bit1)) (bit2) (not (bit3)) (not (bit4)) (not (bit5))
      (oneof (and) (not (not-flattire)))
    )
  )

  (:action move_bwd_p4
    :parameters ()
    :precondition (and (not (bit0)) (not (bit1)) (bit2) (not (bit3)) (not (bit4)) (not (bit5)) (not-flattire))
    :effect (and
      (bit0) (bit1) (not (bit2)) (not (bit3)) (not (bit4)) (not (bit5))
      (oneof (and) (not (not-flattire)))
    )
  )

  (:action move_fwd_p5
    :parameters ()
    :precondition (and (bit0) (not (bit1)) (bit2) (not (bit3)) (not (bit4)) (not (bit5)) (not-flattire))
    :effect (and
      (not (bit0)) (bit1) (bit2) (not (bit3)) (not (bit4)) (not (bit5))
      (oneof (and) (not (not-flattire)))
    )
  )

  (:action move_bwd_p5
    :parameters ()
    :precondition (and (bit0) (not (bit1)) (bit2) (not (bit3)) (not (bit4)) (not (bit5)) (not-flattire))
    :effect (and
      (not (bit0)) (not (bit1)) (bit2) (not (bit3)) (not (bit4)) (not (bit5))
      (oneof (and) (not (not-flattire)))
    )
  )

  (:action move_fwd_p6
    :parameters ()
    :precondition (and (not (bit0)) (bit1) (bit2) (not (bit3)) (not (bit4)) (not (bit5)) (not-flattire))
    :effect (and
      (bit0) (bit1) (bit2) (not (bit3)) (not (bit4)) (not (bit5))
      (oneof (and) (not (not-flattire)))
    )
  )

  (:action move_bwd_p6
    :parameters ()
    :precondition (and (not (bit0)) (bit1) (bit2) (not (bit3)) (not (bit4)) (not (bit5)) (not-flattire))
    :effect (and
      (bit0) (not (bit1)) (bit2) (not (bit3)) (not (bit4)) (not (bit5))
      (oneof (and) (not (not-flattire)))
    )
  )

  (:action move_fwd_p7
    :parameters ()
    :precondition (and (bit0) (bit1) (bit2) (not (bit3)) (not (bit4)) (not (bit5)) (not-flattire))
    :effect (and
      (not (bit0)) (not (bit1)) (not (bit2)) (bit3) (not (bit4)) (not (bit5))
      (oneof (and) (not (not-flattire)))
    )
  )

  (:action move_bwd_p7
    :parameters ()
    :precondition (and (bit0) (bit1) (bit2) (not (bit3)) (not (bit4)) (not (bit5)) (not-flattire))
    :effect (and
      (not (bit0)) (bit1) (bit2) (not (bit3)) (not (bit4)) (not (bit5))
      (oneof (and) (not (not-flattire)))
    )
  )

  (:action move_fwd_p8
    :parameters ()
    :precondition (and (not (bit0)) (not (bit1)) (not (bit2)) (bit3) (not (bit4)) (not (bit5)) (not-flattire))
    :effect (and
      (bit0) (not (bit1)) (not (bit2)) (bit3) (not (bit4)) (not (bit5))
      (oneof (and) (not (not-flattire)))
    )
  )

  (:action move_bwd_p8
    :parameters ()
    :precondition (and (not (bit0)) (not (bit1)) (not (bit2)) (bit3) (not (bit4)) (not (bit5)) (not-flattire))
    :effect (and
      (bit0) (bit1) (bit2) (not (bit3)) (not (bit4)) (not (bit5))
      (oneof (and) (not (not-flattire)))
    )
  )

  (:action move_fwd_p9
    :parameters ()
    :precondition (and (bit0) (not (bit1)) (not (bit2)) (bit3) (not (bit4)) (not (bit5)) (not-flattire))
    :effect (and
      (not (bit0)) (bit1) (not (bit2)) (bit3) (not (bit4)) (not (bit5))
      (oneof (and) (not (not-flattire)))
    )
  )

  (:action move_bwd_p9
    :parameters ()
    :precondition (and (bit0) (not (bit1)) (not (bit2)) (bit3) (not (bit4)) (not (bit5)) (not-flattire))
    :effect (and
      (not (bit0)) (not (bit1)) (not (bit2)) (bit3) (not (bit4)) (not (bit5))
      (oneof (and) (not (not-flattire)))
    )
  )

  (:action move_fwd_p10
    :parameters ()
    :precondition (and (not (bit0)) (bit1) (not (bit2)) (bit3) (not (bit4)) (not (bit5)) (not-flattire))
    :effect (and
      (bit0) (bit1) (not (bit2)) (bit3) (not (bit4)) (not (bit5))
      (oneof (and) (not (not-flattire)))
    )
  )

  (:action move_bwd_p10
    :parameters ()
    :precondition (and (not (bit0)) (bit1) (not (bit2)) (bit3) (not (bit4)) (not (bit5)) (not-flattire))
    :effect (and
      (bit0) (not (bit1)) (not (bit2)) (bit3) (not (bit4)) (not (bit5))
      (oneof (and) (not (not-flattire)))
    )
  )

  (:action move_fwd_p11
    :parameters ()
    :precondition (and (bit0) (bit1) (not (bit2)) (bit3) (not (bit4)) (not (bit5)) (not-flattire))
    :effect (and
      (not (bit0)) (not (bit1)) (bit2) (bit3) (not (bit4)) (not (bit5))
      (oneof (and) (not (not-flattire)))
    )
  )

  (:action move_bwd_p11
    :parameters ()
    :precondition (and (bit0) (bit1) (not (bit2)) (bit3) (not (bit4)) (not (bit5)) (not-flattire))
    :effect (and
      (not (bit0)) (bit1) (not (bit2)) (bit3) (not (bit4)) (not (bit5))
      (oneof (and) (not (not-flattire)))
    )
  )

  (:action move_fwd_p12
    :parameters ()
    :precondition (and (not (bit0)) (not (bit1)) (bit2) (bit3) (not (bit4)) (not (bit5)) (not-flattire))
    :effect (and
      (bit0) (not (bit1)) (bit2) (bit3) (not (bit4)) (not (bit5))
      (oneof (and) (not (not-flattire)))
    )
  )

  (:action move_bwd_p12
    :parameters ()
    :precondition (and (not (bit0)) (not (bit1)) (bit2) (bit3) (not (bit4)) (not (bit5)) (not-flattire))
    :effect (and
      (bit0) (bit1) (not (bit2)) (bit3) (not (bit4)) (not (bit5))
      (oneof (and) (not (not-flattire)))
    )
  )

  (:action move_fwd_p13
    :parameters ()
    :precondition (and (bit0) (not (bit1)) (bit2) (bit3) (not (bit4)) (not (bit5)) (not-flattire))
    :effect (and
      (not (bit0)) (bit1) (bit2) (bit3) (not (bit4)) (not (bit5))
      (oneof (and) (not (not-flattire)))
    )
  )

  (:action move_bwd_p13
    :parameters ()
    :precondition (and (bit0) (not (bit1)) (bit2) (bit3) (not (bit4)) (not (bit5)) (not-flattire))
    :effect (and
      (not (bit0)) (not (bit1)) (bit2) (bit3) (not (bit4)) (not (bit5))
      (oneof (and) (not (not-flattire)))
    )
  )

  (:action move_fwd_p14
    :parameters ()
    :precondition (and (not (bit0)) (bit1) (bit2) (bit3) (not (bit4)) (not (bit5)) (not-flattire))
    :effect (and
      (bit0) (bit1) (bit2) (bit3) (not (bit4)) (not (bit5))
      (oneof (and) (not (not-flattire)))
    )
  )

  (:action move_bwd_p14
    :parameters ()
    :precondition (and (not (bit0)) (bit1) (bit2) (bit3) (not (bit4)) (not (bit5)) (not-flattire))
    :effect (and
      (bit0) (not (bit1)) (bit2) (bit3) (not (bit4)) (not (bit5))
      (oneof (and) (not (not-flattire)))
    )
  )

  (:action move_fwd_p15
    :parameters ()
    :precondition (and (bit0) (bit1) (bit2) (bit3) (not (bit4)) (not (bit5)) (not-flattire))
    :effect (and
      (not (bit0)) (not (bit1)) (not (bit2)) (not (bit3)) (bit4) (not (bit5))
      (oneof (and) (not (not-flattire)))
    )
  )

  (:action move_bwd_p15
    :parameters ()
    :precondition (and (bit0) (bit1) (bit2) (bit3) (not (bit4)) (not (bit5)) (not-flattire))
    :effect (and
      (not (bit0)) (bit1) (bit2) (bit3) (not (bit4)) (not (bit5))
      (oneof (and) (not (not-flattire)))
    )
  )

  (:action move_fwd_p16
    :parameters ()
    :precondition (and (not (bit0)) (not (bit1)) (not (bit2)) (not (bit3)) (bit4) (not (bit5)) (not-flattire))
    :effect (and
      (bit0) (not (bit1)) (not (bit2)) (not (bit3)) (bit4) (not (bit5))
      (oneof (and) (not (not-flattire)))
    )
  )

  (:action move_bwd_p16
    :parameters ()
    :precondition (and (not (bit0)) (not (bit1)) (not (bit2)) (not (bit3)) (bit4) (not (bit5)) (not-flattire))
    :effect (and
      (bit0) (bit1) (bit2) (bit3) (not (bit4)) (not (bit5))
      (oneof (and) (not (not-flattire)))
    )
  )

  (:action move_fwd_p17
    :parameters ()
    :precondition (and (bit0) (not (bit1)) (not (bit2)) (not (bit3)) (bit4) (not (bit5)) (not-flattire))
    :effect (and
      (not (bit0)) (bit1) (not (bit2)) (not (bit3)) (bit4) (not (bit5))
      (oneof (and) (not (not-flattire)))
    )
  )

  (:action move_bwd_p17
    :parameters ()
    :precondition (and (bit0) (not (bit1)) (not (bit2)) (not (bit3)) (bit4) (not (bit5)) (not-flattire))
    :effect (and
      (not (bit0)) (not (bit1)) (not (bit2)) (not (bit3)) (bit4) (not (bit5))
      (oneof (and) (not (not-flattire)))
    )
  )

  (:action move_fwd_p18
    :parameters ()
    :precondition (and (not (bit0)) (bit1) (not (bit2)) (not (bit3)) (bit4) (not (bit5)) (not-flattire))
    :effect (and
      (bit0) (bit1) (not (bit2)) (not (bit3)) (bit4) (not (bit5))
      (oneof (and) (not (not-flattire)))
    )
  )

  (:action move_bwd_p18
    :parameters ()
    :precondition (and (not (bit0)) (bit1) (not (bit2)) (not (bit3)) (bit4) (not (bit5)) (not-flattire))
    :effect (and
      (bit0) (not (bit1)) (not (bit2)) (not (bit3)) (bit4) (not (bit5))
      (oneof (and) (not (not-flattire)))
    )
  )

  (:action move_fwd_p19
    :parameters ()
    :precondition (and (bit0) (bit1) (not (bit2)) (not (bit3)) (bit4) (not (bit5)) (not-flattire))
    :effect (and
      (not (bit0)) (not (bit1)) (bit2) (not (bit3)) (bit4) (not (bit5))
      (oneof (and) (not (not-flattire)))
    )
  )

  (:action move_bwd_p19
    :parameters ()
    :precondition (and (bit0) (bit1) (not (bit2)) (not (bit3)) (bit4) (not (bit5)) (not-flattire))
    :effect (and
      (not (bit0)) (bit1) (not (bit2)) (not (bit3)) (bit4) (not (bit5))
      (oneof (and) (not (not-flattire)))
    )
  )

  (:action move_fwd_p20
    :parameters ()
    :precondition (and (not (bit0)) (not (bit1)) (bit2) (not (bit3)) (bit4) (not (bit5)) (not-flattire))
    :effect (and
      (bit0) (not (bit1)) (bit2) (not (bit3)) (bit4) (not (bit5))
      (oneof (and) (not (not-flattire)))
    )
  )

  (:action move_bwd_p20
    :parameters ()
    :precondition (and (not (bit0)) (not (bit1)) (bit2) (not (bit3)) (bit4) (not (bit5)) (not-flattire))
    :effect (and
      (bit0) (bit1) (not (bit2)) (not (bit3)) (bit4) (not (bit5))
      (oneof (and) (not (not-flattire)))
    )
  )

  (:action move_fwd_p21
    :parameters ()
    :precondition (and (bit0) (not (bit1)) (bit2) (not (bit3)) (bit4) (not (bit5)) (not-flattire))
    :effect (and
      (not (bit0)) (bit1) (bit2) (not (bit3)) (bit4) (not (bit5))
      (oneof (and) (not (not-flattire)))
    )
  )

  (:action move_bwd_p21
    :parameters ()
    :precondition (and (bit0) (not (bit1)) (bit2) (not (bit3)) (bit4) (not (bit5)) (not-flattire))
    :effect (and
      (not (bit0)) (not (bit1)) (bit2) (not (bit3)) (bit4) (not (bit5))
      (oneof (and) (not (not-flattire)))
    )
  )

  (:action move_fwd_p22
    :parameters ()
    :precondition (and (not (bit0)) (bit1) (bit2) (not (bit3)) (bit4) (not (bit5)) (not-flattire))
    :effect (and
      (bit0) (bit1) (bit2) (not (bit3)) (bit4) (not (bit5))
      (oneof (and) (not (not-flattire)))
    )
  )

  (:action move_bwd_p22
    :parameters ()
    :precondition (and (not (bit0)) (bit1) (bit2) (not (bit3)) (bit4) (not (bit5)) (not-flattire))
    :effect (and
      (bit0) (not (bit1)) (bit2) (not (bit3)) (bit4) (not (bit5))
      (oneof (and) (not (not-flattire)))
    )
  )

  (:action move_fwd_p23
    :parameters ()
    :precondition (and (bit0) (bit1) (bit2) (not (bit3)) (bit4) (not (bit5)) (not-flattire))
    :effect (and
      (not (bit0)) (not (bit1)) (not (bit2)) (bit3) (bit4) (not (bit5))
      (oneof (and) (not (not-flattire)))
    )
  )

  (:action move_bwd_p23
    :parameters ()
    :precondition (and (bit0) (bit1) (bit2) (not (bit3)) (bit4) (not (bit5)) (not-flattire))
    :effect (and
      (not (bit0)) (bit1) (bit2) (not (bit3)) (bit4) (not (bit5))
      (oneof (and) (not (not-flattire)))
    )
  )

  (:action move_fwd_p24
    :parameters ()
    :precondition (and (not (bit0)) (not (bit1)) (not (bit2)) (bit3) (bit4) (not (bit5)) (not-flattire))
    :effect (and
      (bit0) (not (bit1)) (not (bit2)) (bit3) (bit4) (not (bit5))
      (oneof (and) (not (not-flattire)))
    )
  )

  (:action move_bwd_p24
    :parameters ()
    :precondition (and (not (bit0)) (not (bit1)) (not (bit2)) (bit3) (bit4) (not (bit5)) (not-flattire))
    :effect (and
      (bit0) (bit1) (bit2) (not (bit3)) (bit4) (not (bit5))
      (oneof (and) (not (not-flattire)))
    )
  )

  (:action move_fwd_p25
    :parameters ()
    :precondition (and (bit0) (not (bit1)) (not (bit2)) (bit3) (bit4) (not (bit5)) (not-flattire))
    :effect (and
      (not (bit0)) (bit1) (not (bit2)) (bit3) (bit4) (not (bit5))
      (oneof (and) (not (not-flattire)))
    )
  )

  (:action move_bwd_p25
    :parameters ()
    :precondition (and (bit0) (not (bit1)) (not (bit2)) (bit3) (bit4) (not (bit5)) (not-flattire))
    :effect (and
      (not (bit0)) (not (bit1)) (not (bit2)) (bit3) (bit4) (not (bit5))
      (oneof (and) (not (not-flattire)))
    )
  )

  (:action move_fwd_p26
    :parameters ()
    :precondition (and (not (bit0)) (bit1) (not (bit2)) (bit3) (bit4) (not (bit5)) (not-flattire))
    :effect (and
      (bit0) (bit1) (not (bit2)) (bit3) (bit4) (not (bit5))
      (oneof (and) (not (not-flattire)))
    )
  )

  (:action move_bwd_p26
    :parameters ()
    :precondition (and (not (bit0)) (bit1) (not (bit2)) (bit3) (bit4) (not (bit5)) (not-flattire))
    :effect (and
      (bit0) (not (bit1)) (not (bit2)) (bit3) (bit4) (not (bit5))
      (oneof (and) (not (not-flattire)))
    )
  )

  (:action move_fwd_p27
    :parameters ()
    :precondition (and (bit0) (bit1) (not (bit2)) (bit3) (bit4) (not (bit5)) (not-flattire))
    :effect (and
      (not (bit0)) (not (bit1)) (bit2) (bit3) (bit4) (not (bit5))
      (oneof (and) (not (not-flattire)))
    )
  )

  (:action move_bwd_p27
    :parameters ()
    :precondition (and (bit0) (bit1) (not (bit2)) (bit3) (bit4) (not (bit5)) (not-flattire))
    :effect (and
      (not (bit0)) (bit1) (not (bit2)) (bit3) (bit4) (not (bit5))
      (oneof (and) (not (not-flattire)))
    )
  )

  (:action move_fwd_p28
    :parameters ()
    :precondition (and (not (bit0)) (not (bit1)) (bit2) (bit3) (bit4) (not (bit5)) (not-flattire))
    :effect (and
      (bit0) (not (bit1)) (bit2) (bit3) (bit4) (not (bit5))
      (oneof (and) (not (not-flattire)))
    )
  )

  (:action move_bwd_p28
    :parameters ()
    :precondition (and (not (bit0)) (not (bit1)) (bit2) (bit3) (bit4) (not (bit5)) (not-flattire))
    :effect (and
      (bit0) (bit1) (not (bit2)) (bit3) (bit4) (not (bit5))
      (oneof (and) (not (not-flattire)))
    )
  )

  (:action move_fwd_p29
    :parameters ()
    :precondition (and (bit0) (not (bit1)) (bit2) (bit3) (bit4) (not (bit5)) (not-flattire))
    :effect (and
      (not (bit0)) (bit1) (bit2) (bit3) (bit4) (not (bit5))
      (oneof (and) (not (not-flattire)))
    )
  )

  (:action move_bwd_p29
    :parameters ()
    :precondition (and (bit0) (not (bit1)) (bit2) (bit3) (bit4) (not (bit5)) (not-flattire))
    :effect (and
      (not (bit0)) (not (bit1)) (bit2) (bit3) (bit4) (not (bit5))
      (oneof (and) (not (not-flattire)))
    )
  )

  (:action move_fwd_p30
    :parameters ()
    :precondition (and (not (bit0)) (bit1) (bit2) (bit3) (bit4) (not (bit5)) (not-flattire))
    :effect (and
      (bit0) (bit1) (bit2) (bit3) (bit4) (not (bit5))
      (oneof (and) (not (not-flattire)))
    )
  )

  (:action move_bwd_p30
    :parameters ()
    :precondition (and (not (bit0)) (bit1) (bit2) (bit3) (bit4) (not (bit5)) (not-flattire))
    :effect (and
      (bit0) (not (bit1)) (bit2) (bit3) (bit4) (not (bit5))
      (oneof (and) (not (not-flattire)))
    )
  )

  (:action move_fwd_p31
    :parameters ()
    :precondition (and (bit0) (bit1) (bit2) (bit3) (bit4) (not (bit5)) (not-flattire))
    :effect (and
      (not (bit0)) (not (bit1)) (not (bit2)) (not (bit3)) (not (bit4)) (bit5)
      (oneof (and) (not (not-flattire)))
    )
  )

  (:action move_bwd_p31
    :parameters ()
    :precondition (and (bit0) (bit1) (bit2) (bit3) (bit4) (not (bit5)) (not-flattire))
    :effect (and
      (not (bit0)) (bit1) (bit2) (bit3) (bit4) (not (bit5))
      (oneof (and) (not (not-flattire)))
    )
  )

  (:action move_fwd_p32
    :parameters ()
    :precondition (and (not (bit0)) (not (bit1)) (not (bit2)) (not (bit3)) (not (bit4)) (bit5) (not-flattire))
    :effect (and
      (bit0) (not (bit1)) (not (bit2)) (not (bit3)) (not (bit4)) (bit5)
      (oneof (and) (not (not-flattire)))
    )
  )

  (:action move_bwd_p32
    :parameters ()
    :precondition (and (not (bit0)) (not (bit1)) (not (bit2)) (not (bit3)) (not (bit4)) (bit5) (not-flattire))
    :effect (and
      (bit0) (bit1) (bit2) (bit3) (bit4) (not (bit5))
      (oneof (and) (not (not-flattire)))
    )
  )

  (:action move_fwd_p33
    :parameters ()
    :precondition (and (bit0) (not (bit1)) (not (bit2)) (not (bit3)) (not (bit4)) (bit5) (not-flattire))
    :effect (and
      (not (bit0)) (bit1) (not (bit2)) (not (bit3)) (not (bit4)) (bit5)
      (oneof (and) (not (not-flattire)))
    )
  )

  (:action move_bwd_p33
    :parameters ()
    :precondition (and (bit0) (not (bit1)) (not (bit2)) (not (bit3)) (not (bit4)) (bit5) (not-flattire))
    :effect (and
      (not (bit0)) (not (bit1)) (not (bit2)) (not (bit3)) (not (bit4)) (bit5)
      (oneof (and) (not (not-flattire)))
    )
  )

  (:action move_fwd_p34
    :parameters ()
    :precondition (and (not (bit0)) (bit1) (not (bit2)) (not (bit3)) (not (bit4)) (bit5) (not-flattire))
    :effect (and
      (bit0) (bit1) (not (bit2)) (not (bit3)) (not (bit4)) (bit5)
      (oneof (and) (not (not-flattire)))
    )
  )

  (:action move_bwd_p34
    :parameters ()
    :precondition (and (not (bit0)) (bit1) (not (bit2)) (not (bit3)) (not (bit4)) (bit5) (not-flattire))
    :effect (and
      (bit0) (not (bit1)) (not (bit2)) (not (bit3)) (not (bit4)) (bit5)
      (oneof (and) (not (not-flattire)))
    )
  )

  (:action move_fwd_p35
    :parameters ()
    :precondition (and (bit0) (bit1) (not (bit2)) (not (bit3)) (not (bit4)) (bit5) (not-flattire))
    :effect (and
      (not (bit0)) (not (bit1)) (bit2) (not (bit3)) (not (bit4)) (bit5)
      (oneof (and) (not (not-flattire)))
    )
  )

  (:action move_bwd_p35
    :parameters ()
    :precondition (and (bit0) (bit1) (not (bit2)) (not (bit3)) (not (bit4)) (bit5) (not-flattire))
    :effect (and
      (not (bit0)) (bit1) (not (bit2)) (not (bit3)) (not (bit4)) (bit5)
      (oneof (and) (not (not-flattire)))
    )
  )

  (:action move_fwd_p36
    :parameters ()
    :precondition (and (not (bit0)) (not (bit1)) (bit2) (not (bit3)) (not (bit4)) (bit5) (not-flattire))
    :effect (and
      (bit0) (not (bit1)) (bit2) (not (bit3)) (not (bit4)) (bit5)
      (oneof (and) (not (not-flattire)))
    )
  )

  (:action move_bwd_p36
    :parameters ()
    :precondition (and (not (bit0)) (not (bit1)) (bit2) (not (bit3)) (not (bit4)) (bit5) (not-flattire))
    :effect (and
      (bit0) (bit1) (not (bit2)) (not (bit3)) (not (bit4)) (bit5)
      (oneof (and) (not (not-flattire)))
    )
  )

  (:action move_fwd_p37
    :parameters ()
    :precondition (and (bit0) (not (bit1)) (bit2) (not (bit3)) (not (bit4)) (bit5) (not-flattire))
    :effect (and
      (not (bit0)) (bit1) (bit2) (not (bit3)) (not (bit4)) (bit5)
      (oneof (and) (not (not-flattire)))
    )
  )

  (:action move_bwd_p37
    :parameters ()
    :precondition (and (bit0) (not (bit1)) (bit2) (not (bit3)) (not (bit4)) (bit5) (not-flattire))
    :effect (and
      (not (bit0)) (not (bit1)) (bit2) (not (bit3)) (not (bit4)) (bit5)
      (oneof (and) (not (not-flattire)))
    )
  )

  (:action move_fwd_p38
    :parameters ()
    :precondition (and (not (bit0)) (bit1) (bit2) (not (bit3)) (not (bit4)) (bit5) (not-flattire))
    :effect (and
      (bit0) (bit1) (bit2) (not (bit3)) (not (bit4)) (bit5)
      (oneof (and) (not (not-flattire)))
    )
  )

  (:action move_bwd_p38
    :parameters ()
    :precondition (and (not (bit0)) (bit1) (bit2) (not (bit3)) (not (bit4)) (bit5) (not-flattire))
    :effect (and
      (bit0) (not (bit1)) (bit2) (not (bit3)) (not (bit4)) (bit5)
      (oneof (and) (not (not-flattire)))
    )
  )

  (:action move_fwd_p39
    :parameters ()
    :precondition (and (bit0) (bit1) (bit2) (not (bit3)) (not (bit4)) (bit5) (not-flattire))
    :effect (and
      (not (bit0)) (not (bit1)) (not (bit2)) (bit3) (not (bit4)) (bit5)
      (oneof (and) (not (not-flattire)))
    )
  )

  (:action move_bwd_p39
    :parameters ()
    :precondition (and (bit0) (bit1) (bit2) (not (bit3)) (not (bit4)) (bit5) (not-flattire))
    :effect (and
      (not (bit0)) (bit1) (bit2) (not (bit3)) (not (bit4)) (bit5)
      (oneof (and) (not (not-flattire)))
    )
  )

  (:action move_fwd_p40
    :parameters ()
    :precondition (and (not (bit0)) (not (bit1)) (not (bit2)) (bit3) (not (bit4)) (bit5) (not-flattire))
    :effect (and
      (bit0) (not (bit1)) (not (bit2)) (bit3) (not (bit4)) (bit5)
      (oneof (and) (not (not-flattire)))
    )
  )

  (:action move_bwd_p40
    :parameters ()
    :precondition (and (not (bit0)) (not (bit1)) (not (bit2)) (bit3) (not (bit4)) (bit5) (not-flattire))
    :effect (and
      (bit0) (bit1) (bit2) (not (bit3)) (not (bit4)) (bit5)
      (oneof (and) (not (not-flattire)))
    )
  )

  (:action move_fwd_p41
    :parameters ()
    :precondition (and (bit0) (not (bit1)) (not (bit2)) (bit3) (not (bit4)) (bit5) (not-flattire))
    :effect (and
      (not (bit0)) (bit1) (not (bit2)) (bit3) (not (bit4)) (bit5)
      (oneof (and) (not (not-flattire)))
    )
  )

  (:action move_bwd_p41
    :parameters ()
    :precondition (and (bit0) (not (bit1)) (not (bit2)) (bit3) (not (bit4)) (bit5) (not-flattire))
    :effect (and
      (not (bit0)) (not (bit1)) (not (bit2)) (bit3) (not (bit4)) (bit5)
      (oneof (and) (not (not-flattire)))
    )
  )

  (:action move_fwd_p42
    :parameters ()
    :precondition (and (not (bit0)) (bit1) (not (bit2)) (bit3) (not (bit4)) (bit5) (not-flattire))
    :effect (and
      (bit0) (bit1) (not (bit2)) (bit3) (not (bit4)) (bit5)
      (oneof (and) (not (not-flattire)))
    )
  )

  (:action move_bwd_p42
    :parameters ()
    :precondition (and (not (bit0)) (bit1) (not (bit2)) (bit3) (not (bit4)) (bit5) (not-flattire))
    :effect (and
      (bit0) (not (bit1)) (not (bit2)) (bit3) (not (bit4)) (bit5)
      (oneof (and) (not (not-flattire)))
    )
  )

  (:action move_fwd_p43
    :parameters ()
    :precondition (and (bit0) (bit1) (not (bit2)) (bit3) (not (bit4)) (bit5) (not-flattire))
    :effect (and
      (not (bit0)) (not (bit1)) (bit2) (bit3) (not (bit4)) (bit5)
      (oneof (and) (not (not-flattire)))
    )
  )

  (:action move_bwd_p43
    :parameters ()
    :precondition (and (bit0) (bit1) (not (bit2)) (bit3) (not (bit4)) (bit5) (not-flattire))
    :effect (and
      (not (bit0)) (bit1) (not (bit2)) (bit3) (not (bit4)) (bit5)
      (oneof (and) (not (not-flattire)))
    )
  )

  (:action move_fwd_p44
    :parameters ()
    :precondition (and (not (bit0)) (not (bit1)) (bit2) (bit3) (not (bit4)) (bit5) (not-flattire))
    :effect (and
      (bit0) (not (bit1)) (bit2) (bit3) (not (bit4)) (bit5)
      (oneof (and) (not (not-flattire)))
    )
  )

  (:action move_bwd_p44
    :parameters ()
    :precondition (and (not (bit0)) (not (bit1)) (bit2) (bit3) (not (bit4)) (bit5) (not-flattire))
    :effect (and
      (bit0) (bit1) (not (bit2)) (bit3) (not (bit4)) (bit5)
      (oneof (and) (not (not-flattire)))
    )
  )

  (:action move_fwd_p45
    :parameters ()
    :precondition (and (bit0) (not (bit1)) (bit2) (bit3) (not (bit4)) (bit5) (not-flattire))
    :effect (and
      (not (bit0)) (bit1) (bit2) (bit3) (not (bit4)) (bit5)
      (oneof (and) (not (not-flattire)))
    )
  )

  (:action move_bwd_p45
    :parameters ()
    :precondition (and (bit0) (not (bit1)) (bit2) (bit3) (not (bit4)) (bit5) (not-flattire))
    :effect (and
      (not (bit0)) (not (bit1)) (bit2) (bit3) (not (bit4)) (bit5)
      (oneof (and) (not (not-flattire)))
    )
  )

  (:action move_fwd_p46
    :parameters ()
    :precondition (and (not (bit0)) (bit1) (bit2) (bit3) (not (bit4)) (bit5) (not-flattire))
    :effect (and
      (bit0) (bit1) (bit2) (bit3) (not (bit4)) (bit5)
      (oneof (and) (not (not-flattire)))
    )
  )

  (:action move_bwd_p46
    :parameters ()
    :precondition (and (not (bit0)) (bit1) (bit2) (bit3) (not (bit4)) (bit5) (not-flattire))
    :effect (and
      (bit0) (not (bit1)) (bit2) (bit3) (not (bit4)) (bit5)
      (oneof (and) (not (not-flattire)))
    )
  )

  (:action move_fwd_p47
    :parameters ()
    :precondition (and (bit0) (bit1) (bit2) (bit3) (not (bit4)) (bit5) (not-flattire))
    :effect (and
      (not (bit0)) (not (bit1)) (not (bit2)) (not (bit3)) (bit4) (bit5)
      (oneof (and) (not (not-flattire)))
    )
  )

  (:action move_bwd_p47
    :parameters ()
    :precondition (and (bit0) (bit1) (bit2) (bit3) (not (bit4)) (bit5) (not-flattire))
    :effect (and
      (not (bit0)) (bit1) (bit2) (bit3) (not (bit4)) (bit5)
      (oneof (and) (not (not-flattire)))
    )
  )

  (:action move_fwd_p48
    :parameters ()
    :precondition (and (not (bit0)) (not (bit1)) (not (bit2)) (not (bit3)) (bit4) (bit5) (not-flattire))
    :effect (and
      (bit0) (not (bit1)) (not (bit2)) (not (bit3)) (bit4) (bit5)
      (oneof (and) (not (not-flattire)))
    )
  )

  (:action move_bwd_p48
    :parameters ()
    :precondition (and (not (bit0)) (not (bit1)) (not (bit2)) (not (bit3)) (bit4) (bit5) (not-flattire))
    :effect (and
      (bit0) (bit1) (bit2) (bit3) (not (bit4)) (bit5)
      (oneof (and) (not (not-flattire)))
    )
  )

  (:action move_bwd_p49
    :parameters ()
    :precondition (and (bit0) (not (bit1)) (not (bit2)) (not (bit3)) (bit4) (bit5) (not-flattire))
    :effect (and
      (not (bit0)) (not (bit1)) (not (bit2)) (not (bit3)) (bit4) (bit5)
      (oneof (and) (not (not-flattire)))
    )
  )

)
