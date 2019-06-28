using System.ComponentModel.Composition;
using Microsoft.VisualStudio.Text;
using Microsoft.VisualStudio.Text.Classification;
using Microsoft.VisualStudio.Utilities;

namespace UXX.Language
{
    [Export(typeof(IClassifierProvider))]
    [ContentType("U++")]
    internal class EditorClassifierProvider : IClassifierProvider
    {
#pragma warning disable 649

        [Import]
        private IClassificationTypeRegistryService classificationRegistry;

#pragma warning restore 649

        #region IClassifierProvider

        public IClassifier GetClassifier(ITextBuffer buffer)
        {
            return buffer.Properties.GetOrCreateSingletonProperty<EditorClassifier>(creator: () => new EditorClassifier(this.classificationRegistry, buffer));
        }

        #endregion
    }
}
