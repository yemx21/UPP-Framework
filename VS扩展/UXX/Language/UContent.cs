using Microsoft.VisualStudio.Text;
using Microsoft.VisualStudio.Text.Classification;
using Microsoft.VisualStudio.Text.Tagging;
using Microsoft.VisualStudio.Utilities;
using System;
using System.Collections.Generic;
using System.ComponentModel.Composition;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace UXX.Language
{
    internal static class UhUppContentTypeDefinitions
    {
        [Export]
        [Name("U++")]
        [BaseDefinition("C/C++")]
        internal static ContentTypeDefinition UContentTypeDefinition = null;

        [Export]
        [FileExtension(".uh")]
        [ContentType("U++")]
        internal static FileExtensionToContentTypeDefinition UHeaderFileExtensionDefinition = null;

        [Export]
        [FileExtension(".upp")]
        [ContentType("U++")]
        internal static FileExtensionToContentTypeDefinition UCodeFileExtensionDefinition = null;

        [Export]
        [FileExtension(".h")]
        [ContentType("U++")]
        internal static FileExtensionToContentTypeDefinition CXXHeaderFileExtensionDefinition = null;

        [Export]
        [FileExtension(".hpp")]
        [ContentType("U++")]
        internal static FileExtensionToContentTypeDefinition CXXHeaderSourceFileExtensionDefinition = null;

        [Export]
        [FileExtension(".cpp")]
        [ContentType("U++")]
        internal static FileExtensionToContentTypeDefinition CXXCodeFileExtensionDefinition = null;

    }

    public enum ULanguageTokenTypes
    {
        Keyword,
    }
    public static class ULanguage
    {
        public static bool Classify(ULanguageTokenTypes tokenType, SnapshotSpan current, IClassificationType tag, ref List<ClassificationSpan> res)
        {
            bool found = false;
            switch (tokenType)
            {
                case ULanguageTokenTypes.Keyword:
                {
                    if (Keywords.Contains(current.GetText()))
                    {
                        res.Add(new ClassificationSpan(current, tag));
                        found = true;
                    }
                }
                break;
            }
            return found;
        }

        public static string[] Keywords { get; } = {
             "__uenum",
             "__typeof",
             "__uobjdef",
             "__uclass",
             "__uref",
             "__uvalue_begin",
             "__uvalue",
             "__uvalue_end",
             "__uobject",
             "__ufield",
             "__uproperty",
             "__uenum_flag",
        };
    }  
}
