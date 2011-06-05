; test-case for nested conditions

	.set NOPE	0
	.define	YUP
	
	.ifdef YUP
		.printf "YUP was defined.\n"

		.ifdef NOPE
			.printf "NOPE was also defined.\n"

			.if NOPE
				.printf "NOPE was true\n"
				.exit 1
			.else
				.printf "NOPE was false\n"
			.endif

			.if !NOPE
				.printf "NOPE was false\n"
			.else
				.printf "NOPE was true\n"
				.exit 1
			.endif
		.endif
	.endif

	.undefine YUP
	.ifdef YUP
		.printf "BUG!\n"
		.exit 1
	.endif

; testing variable in combination with a recursive macro

	.macro OMG B
		.if A==B && B==0
			.printf "A==B==%d\n", B
			.set A 1
			OMG 1
		.elif A==B && B==1
			.printf "A==B==%d\n", B
			.set A 2
			OMG 2
		.elif A==B && B==2
			.printf "A==B==%d\n", B
		.else
			.printf "Something went wrong!\n"
		.endif
	.endm

	.set A 0
	OMG 0

