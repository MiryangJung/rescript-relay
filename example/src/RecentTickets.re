module Fragment = [%relay.fragment
  {|
  fragment RecentTickets_query on Query
    @refetchable(queryName: "RecentTicketsRefetchQuery")
    @argumentDefinitions(
      first: {type: "Int!", defaultValue: 2},
      after: {type: "String!", defaultValue: ""}
    ) {
    ticketsConnection(first: $first, after: $after)
      @connection(key: "RecentTickets_ticketsConnection")
    {
      pageInfo {
        endCursor
        hasNextPage
      }
      edges {
        node {
          id
          ...SingleTicket_ticket
        }
      }
    }
  }
|}
];

[@react.component]
let make = (~query as queryRef) => {
  let ReasonRelay.{data, hasNext, isLoadingNext, loadNext} =
    Fragment.usePagination(queryRef);

  <div className="card">
    <div className="card-body">
      <h4 className="card-title"> {React.string("Recent Tickets")} </h4>
      <div className="table-responsive">
        <table className="table">
          <thead>
            <tr>
              <th> {React.string("Assignee")} </th>
              <th> {React.string("Subject")} </th>
              <th> {React.string("Status")} </th>
              <th> {React.string("Last Update")} </th>
              <th> {React.string("Tracking ID")} </th>
            </tr>
          </thead>
          <tbody>
            {data##ticketsConnection
             |> ReasonRelayUtils.collectConnectionNodes
             |> Array.map(ticket => <SingleTicket key=ticket##id ticket />)
             |> React.array}
          </tbody>
        </table>
        {hasNext
           ? <button
               className="btn btn-gradient-primary font-weight-bold"
               id="add-task"
               onClick={_ => loadNext(~count=2, ~onComplete=None) |> ignore}
               disabled=isLoadingNext>
               {React.string("More")}
             </button>
           : React.null}
      </div>
    </div>
  </div>;
};