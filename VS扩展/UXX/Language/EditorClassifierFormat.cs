using System.ComponentModel.Composition;
using System.Windows.Media;
using Microsoft.VisualStudio.Text.Classification;
using Microsoft.VisualStudio.Utilities;

namespace UXX.Language
{

    [Export(typeof(EditorFormatDefinition))]
    [ClassificationType(ClassificationTypeNames = "Keywords")]
    [Name("Keywords")]
    [UserVisible(true)]
    [Order(Before = Priority.High, After = Priority.High)]
    internal sealed class EditorClassifierFormat : ClassificationFormatDefinition
    {
        public EditorClassifierFormat()
        {
            this.DisplayName = "U++ Keywords";
            this.ForegroundColor = (Color)ColorConverter.ConvertFromString("#ff6d6d");
        }
    }
}
