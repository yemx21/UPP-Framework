#pragma once

namespace System
{
	namespace Uaml
	{
		enum class UamlError
		{
			OK,
			Failed,

			/*Assembly*/
			AssemblyNotFound,

			/*File*/
			FileNotFound,
			FileOccupied,
			FileCorrupted,

			/*Structure*/
			ElementMismatch,
			DocumentMismatch,
			ElementParsingError,
			AttributeParsingError,
			TextParsingError,
			CDATAParsingError,
			CommentParsingError,
			DeclarationParsingError,
			UnknownParsingError,
		};
	}
}