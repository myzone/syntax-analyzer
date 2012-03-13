#pragma once

#include <QString>

#include "../Utils/Tree.h"
#include "../Events/EventBroadcaster.h"
#include "../Core/Symbol.h"

#include "../defines.h"

namespace Core {

    class TreeAnalyzer {
    private:
        const Events::EventBroadcaster* broadcaster;

        class TreeConverter : public Tree<Symbol>::DataProcessor {
        private:
            mutable QList<Tree<Symbol> > nodes;
        
        public:
            virtual void dataProcessingStarts() const {
                nodes.clear();
            }
            
            virtual void processData(const Tree<Symbol>& node) const {
                nodes.prepend(node);
            }
            
            virtual const TraverseType& getTraverseType() const {
                return TraverseType::WIDTH_TRAVERSE;
            }
            
            const QList<Tree<Symbol> >& getNodes() const {
                return nodes;
            } 
            
        };
       
    public:
        TreeAnalyzer(const Events::EventBroadcaster* broadcaster);
        virtual ~TreeAnalyzer();

        void analyzeTree(const Tree<Symbol>& tree) const;
    };

}

