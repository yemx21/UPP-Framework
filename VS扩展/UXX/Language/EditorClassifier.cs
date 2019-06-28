using System;
using System.Reflection;
using System.Linq;
using System.Collections.Generic;
using Microsoft.VisualStudio.Text;
using Microsoft.VisualStudio.Text.Classification;

namespace UXX.Language
{
    internal class EditorClassifier : IClassifier
    {
        private readonly IClassificationType classificationType;

        private WeakReference<object> vctagger = new WeakReference<object>(null);

        internal EditorClassifier(IClassificationTypeRegistryService registry, ITextBuffer buffer)
        {
            this.classificationType = registry.GetClassificationType("Keywords");

            var vctagger_pair = buffer.Properties.PropertyList.Where(p => p.Value.GetType().FullName == "Microsoft.VisualStudio.VC.VCMacroTagger").FirstOrDefault();
            if(!vctagger_pair.Equals(default(KeyValuePair<object, object>)))
            {
                var vctagger_val = vctagger_pair.Value;
                if(vctagger_val != null)
                {
                    Type vctagger_type = vctagger_val.GetType();
                    this.vctagger.SetTarget(vctagger_val);
                }

            }
        }

        #region IClassifier

#pragma warning disable 67
        public event EventHandler<ClassificationChangedEventArgs> ClassificationChanged;
#pragma warning restore 67

        public IList<ClassificationSpan> GetClassificationSpans(SnapshotSpan span)
        {
            var result = new List<ClassificationSpan>();
            var normal_span = new NormalizedSnapshotSpanCollection(span);

            object tagger = null;
            if(vctagger.TryGetTarget(out tagger))
            {
                var gettags = tagger.GetType().GetMethod("GetTags", BindingFlags.Public | BindingFlags.Instance);
                if (gettags!=null)
                { 
                var tags = gettags.Invoke(tagger, new object[] { normal_span });
                    if (tags != null)
                    {
                        var tags_enumerable = tags as System.Collections.IEnumerable;
                        if (tags_enumerable != null)
                        {
                            foreach (var tag in tags_enumerable)
                            {
                                var get_span = tag.GetType().GetProperty("Span");
                                if (get_span != null)
                                {
                                    var tag_span_obj = get_span.GetValue(tag);
                                    if (tag_span_obj != null)
                                    {
                                        if (tag_span_obj is SnapshotSpan)
                                        {
                                            SnapshotSpan tag_span = (SnapshotSpan)tag_span_obj;
                                            ULanguage.Classify(ULanguageTokenTypes.Keyword, tag_span, classificationType, ref result);
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
            return result;
        }



        #endregion
    }
}
