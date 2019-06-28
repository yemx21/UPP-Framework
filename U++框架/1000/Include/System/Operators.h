#pragma once

namespace System
{
	/// <summary>
	/// 堍呾睫笱濬
	/// </summary>
	enum class Operators
	{
		/*Binary Operators*/
		Comma,									/*,*/
		Inequality,									 /*!=*/
		Modulus,									 /*%*/
		ModulusAssignment,					/*%=*/
		BitwiseAND,								/*&*/
		LogicalAND,								/*&&*/
		BitwiseANDAssignment,				/*&=*/
		Multiplication,							/***/
		MultiplicationAssignment,			 /**=*/
		Addition,									/*+*/
		AdditionAssignment,					 /*+=*/
		Subtraction,								 /*每*/
		SubtractionAssignment,				 /*每 =*/
		MemberSelection,						 /*每>*/
		PointerToMemberSelection,		 /*每>**/
		Division,									/*/*/
		DivisionAssignment,					 /*/=*/
		LessThan,									/*<*/
		LeftShift,									/*<<*/
		LeftShiftAssignment,					/*<<=*/
		LessThanOrEqualTo,					/*<=*/
		Assignment,								/*=*/
		Equality,									/*==*/
		GreaterThan,								 /*>*/
		GreaterThanOrEqualTo,				 /*>= */
		RightShift,									 /*>>*/
		RightShiftAssignment,					 /*>>=*/
		ExclusiveOR,								/*^*/
		ExclusiveORAssignment,				/*^=*/
		BitwiseInclusiveOR,						/*|*/
		BitwiseInclusiveORAssignment,		/*|=*/
		LogicalOR,									/*||*/

		/*Unary Operators*/

		LogicalNOT,								 /*!*/
		AddressOf,								/*&*/
		OnesComplement,						/*~*/
		PointerDereference,					/***/
		UnaryPlus,									/*+*/
		UnaryNegation,							/*-*/
		Increment,									/*++*/
		Decrement,								/*--*/
		ConversionOperators,				/*()*/

		Function,									/*()*/
		Subscripting,                             /*[]*/


		Unknown=0x400
	};

	template<Operators __OP>
	struct is_operatorSupportedWithNoArg
	{
		static const bool value = false;
	};

	template<>
	struct is_operatorSupportedWithNoArg<Operators::MemberSelection>
	{
		static const bool value = true;
	};

	template<>
	struct is_operatorSupportedWithNoArg<Operators::PointerToMemberSelection>
	{
		static const bool value = true;
	};

	template<>
	struct is_operatorSupportedWithNoArg<Operators::LogicalNOT>
	{
		static const bool value = true;
	};

	template<>
	struct is_operatorSupportedWithNoArg<Operators::AddressOf>
	{
		static const bool value = true;
	};

	template<>
	struct is_operatorSupportedWithNoArg<Operators::OnesComplement>
	{
		static const bool value = true;
	};

	template<>
	struct is_operatorSupportedWithNoArg<Operators::PointerDereference>
	{
		static const bool value = true;
	};

	template<>
	struct is_operatorSupportedWithNoArg<Operators::UnaryPlus>
	{
		static const bool value = true;
	};

	template<>
	struct is_operatorSupportedWithNoArg<Operators::UnaryNegation>
	{
		static const bool value = true;
	};

	template<>
	struct is_operatorSupportedWithNoArg<Operators::Increment>
	{
		static const bool value = true;
	};

	template<>
	struct is_operatorSupportedWithNoArg<Operators::Decrement>
	{
		static const bool value = true;
	};

	template<>
	struct is_operatorSupportedWithNoArg<Operators::ConversionOperators>
	{
		static const bool value = true;
	};

	template<Operators __OP>
	struct is_operatorSupportedWithMoreThanOneArgs
	{
		static const bool value = false;
	};

	template<>
	struct is_operatorSupportedWithMoreThanOneArgs<Operators::Function>
	{
		static const bool value = true;
	};


	template<Operators __OP>
	struct is_binaryOperator
	{
		static const bool value = false;
	};

	template<>
	struct is_binaryOperator<Operators::Comma>
	{
		static const bool value = false;
	};

	template<>
	struct is_binaryOperator<Operators::Inequality>
	{
		static const bool value = true;
	};

	template<>
	struct is_binaryOperator<Operators::Modulus>
	{
		static const bool value = true;
	};

	template<>
	struct is_binaryOperator<Operators::ModulusAssignment>
	{
		static const bool value = true;
	};

	template<>
	struct is_binaryOperator<Operators::BitwiseAND>
	{
		static const bool value = true;
	};

	template<>
	struct is_binaryOperator<Operators::LogicalAND>
	{
		static const bool value = true;
	};

	template<>
	struct is_binaryOperator<Operators::BitwiseANDAssignment>
	{
		static const bool value = true;
	};

	template<>
	struct is_binaryOperator<Operators::Multiplication>
	{
		static const bool value = true;
	};

	template<>
	struct is_binaryOperator<Operators::MultiplicationAssignment>
	{
		static const bool value = true;
	};

	template<>
	struct is_binaryOperator<Operators::Addition>
	{
		static const bool value = true;
	};

	template<>
	struct is_binaryOperator<Operators::AdditionAssignment>
	{
		static const bool value = true;
	};

	template<>
	struct is_binaryOperator<Operators::Subtraction>
	{
		static const bool value = true;
	};

	template<>
	struct is_binaryOperator<Operators::SubtractionAssignment>
	{
		static const bool value = true;
	};

	template<>
	struct is_binaryOperator<Operators::MemberSelection>
	{
		static const bool value = true;
	};

	template<>
	struct is_binaryOperator<Operators::PointerToMemberSelection>
	{
		static const bool value = true;
	};

	template<>
	struct is_binaryOperator<Operators::Division>
	{
		static const bool value = true;
	};

	template<>
	struct is_binaryOperator<Operators::DivisionAssignment>
	{
		static const bool value = true;
	};

	template<>
	struct is_binaryOperator<Operators::LessThan>
	{
		static const bool value = true;
	};

	template<>
	struct is_binaryOperator<Operators::LeftShift>
	{
		static const bool value = true;
	};

	template<>
	struct is_binaryOperator<Operators::LeftShiftAssignment>
	{
		static const bool value = true;
	};

	template<>
	struct is_binaryOperator<Operators::LessThanOrEqualTo>
	{
		static const bool value = true;
	};

	template<>
	struct is_binaryOperator<Operators::Assignment>
	{
		static const bool value = true;
	};

	template<>
	struct is_binaryOperator<Operators::Equality>
	{
		static const bool value = true;
	};

	template<>
	struct is_binaryOperator<Operators::GreaterThan>
	{
		static const bool value = true;
	};

	template<>
	struct is_binaryOperator<Operators::GreaterThanOrEqualTo>
	{
		static const bool value = true;
	};

	template<>
	struct is_binaryOperator<Operators::RightShift>
	{
		static const bool value = true;
	};

	template<>
	struct is_binaryOperator<Operators::RightShiftAssignment>
	{
		static const bool value = true;
	};

	template<>
	struct is_binaryOperator<Operators::ExclusiveOR>
	{
		static const bool value = true;
	};

	template<>
	struct is_binaryOperator<Operators::ExclusiveORAssignment>
	{
		static const bool value = true;
	};

	template<>
	struct is_binaryOperator<Operators::BitwiseInclusiveOR>
	{
		static const bool value = true;
	};

	template<>
	struct is_binaryOperator<Operators::BitwiseInclusiveORAssignment>
	{
		static const bool value = true;
	};

	template<>
	struct is_binaryOperator<Operators::LogicalOR>
	{
		static const bool value = true;
	};
}