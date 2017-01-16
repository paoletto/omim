#pragma once

#include "routing/directions_engine.hpp"
#include "routing/loaded_path_segment.hpp"
#include "routing/turn_candidate.hpp"

#include "std/map.hpp"
#include "std/unique_ptr.hpp"

class Index;

namespace routing
{
class BicycleDirectionsEngine : public IDirectionsEngine
{
public:
  struct AdjacentEdges
  {
    explicit AdjacentEdges(size_t ingoingTurnsCount = 0) : m_ingoingTurnsCount(ingoingTurnsCount) {}

    turns::TurnCandidates m_outgoingTurns;
    size_t m_ingoingTurnsCount;
  };

  using AdjacentEdgesMap = map<UniNodeId, AdjacentEdges>;

  BicycleDirectionsEngine(Index const & index);

  // IDirectionsEngine override:
  void Generate(RoadGraphBase const & graph, vector<Junction> const & path, Route::TTimes & times,
                Route::TTurns & turns, vector<Junction> & routeGeometry,
                vector<traffic::TrafficInfo::RoadSegmentId> & trafficSegs,
                my::Cancellable const & cancellable) override;

private:
  Index::FeaturesLoaderGuard & GetLoader(MwmSet::MwmId const & id);
  void LoadPathGeometry(UniNodeId const & uniNodeId, vector<Junction> const & path,
                        LoadedPathSegment & pathSegment);

  AdjacentEdgesMap m_adjacentEdges;
  TUnpackedPathSegments m_pathSegments;
  Index const & m_index;
  unique_ptr<Index::FeaturesLoaderGuard> m_loader;
};
}  // namespace routing
