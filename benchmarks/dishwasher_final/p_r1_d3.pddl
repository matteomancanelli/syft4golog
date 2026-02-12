(define (problem dishwasher_r1_d3)
(:domain warehouse-domain)
(:objects
  d1 d2 d3 - dish
  r1 - room
)
(:init
  (emptyhand)
  (robot-at r0)
  (dish-at d1 r1)
  (dish-at d2 r1)
  (dish-at d3 r1)
)
(:goal (and
  (dish-at d1 r0)
  (dish-at d2 r0)
  (dish-at d3 r0)
))
)