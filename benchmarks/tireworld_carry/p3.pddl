(define (problem tire_carry_3)
  (:domain tire-carry)
  (:init
    (not (bit0))
    (not (bit1))
    (not-flattire)
  )
  (:goal (and (not (bit0)) (bit1)))
)
