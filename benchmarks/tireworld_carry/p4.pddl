(define (problem tire_carry_4)
  (:domain tire-carry)
  (:init
    (not (bit0))
    (not (bit1))
    (not-flattire)
  )
  (:goal (and (bit0) (bit1)))
)
