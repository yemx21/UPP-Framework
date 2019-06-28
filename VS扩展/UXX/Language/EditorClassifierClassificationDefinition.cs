using System.ComponentModel.Composition;
using Microsoft.VisualStudio.Text.Classification;
using Microsoft.VisualStudio.Utilities;

namespace UXX.Language
{

    internal static class EditorClassifierClassificationDefinition
    {
#pragma warning disable 169

        [Export(typeof(ClassificationTypeDefinition))]
        [Name("Keywords")]
        private static ClassificationTypeDefinition KeywordClassificationType;

#pragma warning restore 169
    }
}
