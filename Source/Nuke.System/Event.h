


namespace Nuke::System
{
	template <typename EventArgs>
	class Event
	{
	public:
		using EventHandler = std::function<void(EventArgs&)>;
		void AddHandler(EventHandler eventHandler)
		{
			_eventHandlers.push_back(eventHandler);
		}
		void Raise(EventArgs& eventArgs)
		{
			for (const auto& _eventHandler : _eventHandlers)
			{
				_eventHandler(eventArgs);
			}
		}
		std::vector<EventHandler> _eventHandlers;

	};
}